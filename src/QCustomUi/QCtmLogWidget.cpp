#include "QCtmLogWidget.h"
#include "QCtmLogModel.h"
#include "QCtmLogFilterModel.h"
#include "QCtmTableView.h"
#include "QCtmComboBox.h"
#include "Private/QCtmToolButton_p.h"

#include <QWidgetAction>
#include <QTableView>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QApplication>
#include <QClipboard>

struct QCtmLogWidget::Impl
{
    QCtmLogFilterModel* proxyModel{ nullptr };
    QCtmLogModel* model{ nullptr };
    QWidgetAction* searchAction{ nullptr };
    QCtmToolButton* searchButton{ nullptr };
    QCtmComboBox* searchEdit{ nullptr };

    QCtmTableView* logView{ nullptr };
    QAction* infoAction{ nullptr };
    QAction* warningAction{ nullptr };
    QAction* errorAction{ nullptr };
    QAction* clearAction{ nullptr };
    QAction* copyAction{ nullptr };
};

QCtmLogWidget::QCtmLogWidget(const QString& objectName, QWidget *parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    this->setObjectName(objectName);
    init();
}

QCtmLogWidget::~QCtmLogWidget()
{
}

/**
 * @brief		设置日志插入顺序
 * @param[in]	mode 日志插入顺序
 */
void QCtmLogWidget::setLogInsertMode(LogInsertMode mode)
{
    m_impl->model->setLogInsertMode(mode);
}

/**
 * @brief		获取日志插入顺序
 * @Return:		日志插入顺序
 */
LogInsertMode QCtmLogWidget::logInsertMode() const
{
    return m_impl->model->logInsertMode();
}

/**
 * @brief		设置日志列是否显示
 * @param[in]	column 列
 * @param[in]	visible 是否显示
 */
void QCtmLogWidget::setColumnVisible(QCtmLogColumn column, bool visible)
{
    if (column == QCtmLogColumn::Level)
    {
        m_impl->infoAction->setVisible(visible);
        m_impl->warningAction->setVisible(visible);
        m_impl->errorAction->setVisible(visible);
    }
    m_impl->logView->setColumnHidden(int(column), !visible);
}

/**
 * @brief		获取日志列是否显示
 * @param[in]	column 列
 * @Return:		是否显示
 */
bool QCtmLogWidget::columnVisible(QCtmLogColumn column) const
{
    return !m_impl->logView->isColumnHidden(int(column));
}

void QCtmLogWidget::init()
{
    m_impl->errorAction = new QAction(tr("Error %1").arg(0), this);
    m_impl->errorAction->setObjectName("errorAction");
    m_impl->errorAction->setCheckable(true);
    m_impl->errorAction->setChecked(true);
    m_impl->warningAction = new QAction(tr("Warn %1").arg(0), this);
    m_impl->warningAction->setObjectName("warningAction");
    m_impl->warningAction->setCheckable(true);
    m_impl->warningAction->setChecked(true);
    m_impl->infoAction = new QAction(tr("Info %1").arg(0), this);
    m_impl->infoAction->setObjectName("infoAction");
    m_impl->infoAction->setCheckable(true);
    m_impl->infoAction->setChecked(true);
    m_impl->clearAction = new QAction(this);
    m_impl->clearAction->setObjectName("clearAction");
    m_impl->copyAction = new QAction(this);
    m_impl->copyAction->setObjectName("copyAction");
    m_impl->copyAction->setShortcut(QKeySequence::Copy);
    m_impl->copyAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    m_impl->searchEdit = new QCtmComboBox(this);
    m_impl->searchEdit->setObjectName("searchEdit");
    m_impl->searchEdit->setEditable(true);
    m_impl->logView = new QCtmTableView(this);
    m_impl->logView->setObjectName("logView");
    m_impl->logView->verticalHeader()->hide();
    m_impl->logView->horizontalHeader()->setStretchLastSection(true);
    m_impl->logView->setSelectionBehavior(QTableView::SelectRows);
    m_impl->logView->setSelectionMode(QTableView::SingleSelection);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(m_impl->logView);

    addAction(m_impl->errorAction);
    addAction(m_impl->warningAction);
    addAction(m_impl->infoAction);
    auto sep = new QAction(this);
    sep->setSeparator(true);
    addAction(sep);
    addAction(m_impl->clearAction);
    addAction(m_impl->copyAction);

    auto wa = new QWidgetAction(this);
    wa->setDefaultWidget(m_impl->searchEdit);
    addAction(wa);

    m_impl->proxyModel = new QCtmLogFilterModel(this);
    m_impl->proxyModel->showLog(QtMsgType::QtInfoMsg, true);
    m_impl->proxyModel->showLog(QtMsgType::QtWarningMsg, true);
    m_impl->proxyModel->showLog(QtMsgType::QtCriticalMsg, true);
    m_impl->model = new QCtmLogModel(objectName(), this);
    m_impl->proxyModel->setSourceModel(m_impl->model);
    m_impl->logView->setModel(m_impl->proxyModel);
    m_impl->logView->setColumnWidth(0, 25);
    m_impl->logView->setColumnWidth(1, 135);
    m_impl->logView->setWordWrap(true);
    m_impl->logView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    connect(m_impl->infoAction, &QAction::toggled, this
        , [=](bool check)
    {
        m_impl->proxyModel->showLog(QtMsgType::QtInfoMsg, check);
        m_impl->proxyModel->invalidate();
        m_impl->logView->horizontalHeader()->reset();
    });
    connect(m_impl->warningAction, &QAction::toggled, this
        , [=](bool check)
    {
        m_impl->proxyModel->showLog(QtMsgType::QtWarningMsg, check);
        m_impl->proxyModel->invalidate();
        m_impl->logView->horizontalHeader()->reset();
    });
    connect(m_impl->errorAction, &QAction::toggled, this
        , [=](bool check)
    {
        m_impl->proxyModel->showLog(QtMsgType::QtCriticalMsg, check);
        m_impl->proxyModel->invalidate();
        m_impl->logView->horizontalHeader()->reset();
    });

    connect(m_impl->clearAction, &QAction::triggered, this
        , [=]()
    {
        m_impl->model->clear();
    });

    m_impl->searchAction = new QWidgetAction(this);
    m_impl->searchButton = new QCtmToolButton(m_impl->searchEdit);
    m_impl->searchButton->setDefaultAction(m_impl->searchAction);
    m_impl->searchButton->setObjectName("searchAction");
    m_impl->searchAction->setDefaultWidget(m_impl->searchButton);
    m_impl->searchEdit->lineEdit()->addAction(m_impl->searchAction, QLineEdit::ActionPosition::TrailingPosition);
    m_impl->searchAction->setObjectName("searchAction");

    connect(m_impl->searchEdit->lineEdit(), &QLineEdit::returnPressed, this, [=]() {
        emit m_impl->searchAction->triggered(false);
    });

    connect(m_impl->searchAction, &QAction::triggered, this, [=](bool)
    {
        const auto& text = m_impl->searchEdit->currentText();
        m_impl->proxyModel->search(text);
        m_impl->logView->horizontalHeader()->reset();

        if (text.isEmpty())
            return;

        for (int i = m_impl->searchEdit->count() - 1; i >= 0; i--)
        {
            const auto& t = m_impl->searchEdit->itemText(i);
            if (t == text)
            {
                m_impl->searchEdit->removeItem(i);
            }
        }
        m_impl->searchEdit->insertItem(0, text);
        m_impl->searchEdit->setCurrentIndex(0);
        if (m_impl->searchEdit->count() > 10)
        {
            m_impl->searchEdit->removeItem(m_impl->searchEdit->count() - 1);
        }
    });

    connect(m_impl->copyAction, &QAction::triggered, this, [=]() {
        const auto& index = m_impl->logView->currentIndex();
        if (index.isValid())
        {
            auto cb = qApp->clipboard();
            auto text = m_impl->model->data(m_impl->model->index(index.row(), 1), Qt::DisplayRole).toString()
                + " "
                + m_impl->model->data(m_impl->model->index(index.row(), 2), Qt::DisplayRole).toString();
            cb->setText(text);
        }
    });

    connect(m_impl->model, &QAbstractItemModel::rowsInserted, this, &QCtmLogWidget::updateLogCount);
    connect(m_impl->model, &QAbstractItemModel::rowsRemoved, this, &QCtmLogWidget::updateLogCount);
    connect(m_impl->model, &QAbstractItemModel::modelReset, this, &QCtmLogWidget::updateLogCount);
}


void QCtmLogWidget::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        updateLogCount();
    }
}

void QCtmLogWidget::updateLogCount()
{
    m_impl->errorAction->setText(tr("Error %1").arg(m_impl->model->errorCount()));
    m_impl->warningAction->setText(tr("Warn %1").arg(m_impl->model->warningCount()));
    m_impl->infoAction->setText(tr("Info %1").arg(m_impl->model->infoCount()));
}

void QCtmLogWidget::setInfoIcon(const QIcon& icon)
{
    m_impl->model->setInfoIcon(icon);
    m_impl->infoAction->setIcon(icon);
}

const QIcon& QCtmLogWidget::infoIcon() const
{
    return m_impl->model->infoIcon();
}

void QCtmLogWidget::setWarningIcon(const QIcon& icon)
{
    m_impl->model->setWarningIcon(icon);
    m_impl->warningAction->setIcon(icon);
}

const QIcon& QCtmLogWidget::warningIcon() const
{
    return m_impl->model->warningIcon();
}

void QCtmLogWidget::setErrorIcon(const QIcon& icon)
{
    m_impl->model->setErrorIcon(icon);
    m_impl->errorAction->setIcon(icon);
}

const QIcon& QCtmLogWidget::errorIcon() const
{
    return m_impl->model->errorIcon();
}

void QCtmLogWidget::setMaximumCount(int count)
{
    m_impl->model->setMaximumCount(count);
}

int QCtmLogWidget::maximumCount() const
{
    return m_impl->model->maximumCount();
}

