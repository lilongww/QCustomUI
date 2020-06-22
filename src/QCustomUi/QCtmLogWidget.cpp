/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/

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

/*!
    \class      QCtmLogWidget
    \brief      QCtmLogWidget provide a log message view widget to show log message datas.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \property   QCtmLogWidget::infoIcon
    \brief      Holds icon of the information log.
*/

/*!
    \property   QCtmLogWidget::warningIcon
    \brief      Holds icon of the warning log.
*/

/*!
    \property   QCtmLogWidget::errorIcon
    \brief      Holds icon of the error log.
*/

/*!
    \enum       QCtmLogWidget::QCtmLogColumn
                Describe column of the log view.
    \value      Level
                The level of log.
    \value      Time
                The time of log.
    \value      Description
                The description of log.
*/

/*!
    \brief      Constructs a log view widget with \a objectName and \a parent.
*/
QCtmLogWidget::QCtmLogWidget(const QString& objectName, QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    this->setObjectName(objectName);
    init();
}

/*!
    \brief      Destroys the log view widget.
*/
QCtmLogWidget::~QCtmLogWidget()
{
}

/*!
    \brief      Set insert \a policy of the log.
    \sa         logInsertPolicy()
*/
void QCtmLogWidget::setLogInsertPolicy(QCtmLogData::LogInsertPolicy policy)
{
    m_impl->model->setLogInsertPolicy(policy);
}

/*!
    \brief      Returns insert policy of the log.
    \sa         setLogInsertPolicy
*/
QCtmLogData::LogInsertPolicy QCtmLogWidget::logInsertPolicy() const
{
    return m_impl->model->logInsertPolicy();
}

/*!
    \brief      Sets whether the \a column is \a visible.
    \sa         columnVisible
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

/*!
    \brief      Returns whether the \a column is visible.
    \sa         setColumnVisible
*/
bool QCtmLogWidget::columnVisible(QCtmLogColumn column) const
{
    return !m_impl->logView->isColumnHidden(int(column));
}

/*!
    \brief      Initialization
*/
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

    QHBoxLayout* layout = new QHBoxLayout(this);
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

/*!
    \reimp
*/
void QCtmLogWidget::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        updateLogCount();
    }
}

/*!
    \brief      Update the log message counts.
*/
void QCtmLogWidget::updateLogCount()
{
    m_impl->errorAction->setText(tr("Error %1").arg(m_impl->model->errorCount()));
    m_impl->warningAction->setText(tr("Warn %1").arg(m_impl->model->warningCount()));
    m_impl->infoAction->setText(tr("Info %1").arg(m_impl->model->infoCount()));
}

/*!
    \brief      Sets the \a icon for information message type.
    \sa         infoIcon()
*/
void QCtmLogWidget::setInfoIcon(const QIcon& icon)
{
    m_impl->model->setInfoIcon(icon);
    m_impl->infoAction->setIcon(icon);
}

/*!
    \brief      Returns the icon for information message type.
    \sa         setInfoIcon
*/
const QIcon& QCtmLogWidget::infoIcon() const
{
    return m_impl->model->infoIcon();
}

/*!
    \brief      Sets the \a icon for warning message type.
    \sa         warningIcon()
*/
void QCtmLogWidget::setWarningIcon(const QIcon& icon)
{
    m_impl->model->setWarningIcon(icon);
    m_impl->warningAction->setIcon(icon);
}

/*!
    \brief      Returns the icon for warning message type.
    \sa         setWarningIcon
*/
const QIcon& QCtmLogWidget::warningIcon() const
{
    return m_impl->model->warningIcon();
}

/*!
    \brief      Sets the \a icon for error message type.
    \sa         errorIcon()
*/
void QCtmLogWidget::setErrorIcon(const QIcon& icon)
{
    m_impl->model->setErrorIcon(icon);
    m_impl->errorAction->setIcon(icon);
}

/*!
    \brief      Returns the icon for error message type.
    \sa         setErrorIcon
*/
const QIcon& QCtmLogWidget::errorIcon() const
{
    return m_impl->model->errorIcon();
}

/*!
    \brief      Sets the maximum \a count of log message.
    \sa         maximumCount()
*/
void QCtmLogWidget::setMaximumCount(int count)
{
    m_impl->model->setMaximumCount(count);
}

/*!
    \brief      Returns the maximum count of log message.
    \sa         setMaximumCount
*/
int QCtmLogWidget::maximumCount() const
{
    return m_impl->model->maximumCount();
}

