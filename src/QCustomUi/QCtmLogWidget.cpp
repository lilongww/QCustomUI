/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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
#include "Private/QCtmToolButton_p.h"
#include "QCtmComboBox.h"
#include "QCtmLogFilterModel.h"
#include "QCtmLogModel.h"
#include "QCtmTableView.h"

#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QTableView>
#include <QWidgetAction>

struct QCtmLogWidget::Impl
{
    QCtmLogFilterModel* proxyModel { nullptr };
    QCtmLogModel* model { nullptr };
    QWidgetAction* searchAction { nullptr };
    QCtmToolButton* searchButton { nullptr };
    QCtmComboBox* searchEdit { nullptr };

    QCtmTableView* logView { nullptr };
    QAction* infoAction { nullptr };
    QAction* warningAction { nullptr };
    QAction* errorAction { nullptr };
    QAction* clearAction { nullptr };
    QAction* copyAction { nullptr };
};

/*!
    \class      QCtmLogWidget
    \brief      标准日志窗口.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLogWidget.h
*/

/*!
    \property   QCtmLogWidget::infoIcon
    \brief      Info 等级日志图标.
*/

/*!
    \property   QCtmLogWidget::warningIcon
    \brief      Warning 等级日志图标.
*/

/*!
    \property   QCtmLogWidget::errorIcon
    \brief      Error 等级日志图标.
*/

/*!
    \enum       QCtmLogWidget::QCtmLogColumn
                日志列表列定义.
    \value      Level
                日志等级.
    \value      Time
                日志时间.
    \value      Description
                日志描述.
*/

/*!
    \brief      构造函数 \a objectName, \a parent.
*/
QCtmLogWidget::QCtmLogWidget(const QString& objectName, QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    this->setObjectName(objectName);
    init();
}

/*!
    \brief      析构函数.
*/
QCtmLogWidget::~QCtmLogWidget() {}

/*!
    \brief      设置日志插入策略 \a policy.
    \sa         logInsertPolicy()
*/
void QCtmLogWidget::setLogInsertPolicy(QCtmLogData::LogInsertPolicy policy) { m_impl->model->setLogInsertPolicy(policy); }

/*!
    \brief      返回日志插入策略.
    \sa         setLogInsertPolicy
*/
QCtmLogData::LogInsertPolicy QCtmLogWidget::logInsertPolicy() const { return m_impl->model->logInsertPolicy(); }

/*!
    \brief      设置 \a column 列是否可见 \a visible.
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
    \brief      返回 \a column 列是否可见.
    \sa         setColumnVisible
*/
bool QCtmLogWidget::columnVisible(QCtmLogColumn column) const { return !m_impl->logView->isColumnHidden(int(column)); }

/*!
    \brief      初始化.
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
    layout->setContentsMargins(0, 0, 0, 0);
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

    connect(m_impl->infoAction,
            &QAction::toggled,
            this,
            [=](bool check)
            {
                m_impl->proxyModel->showLog(QtMsgType::QtInfoMsg, check);
                m_impl->proxyModel->invalidate();
                m_impl->logView->horizontalHeader()->reset();
            });
    connect(m_impl->warningAction,
            &QAction::toggled,
            this,
            [=](bool check)
            {
                m_impl->proxyModel->showLog(QtMsgType::QtWarningMsg, check);
                m_impl->proxyModel->invalidate();
                m_impl->logView->horizontalHeader()->reset();
            });
    connect(m_impl->errorAction,
            &QAction::toggled,
            this,
            [=](bool check)
            {
                m_impl->proxyModel->showLog(QtMsgType::QtCriticalMsg, check);
                m_impl->proxyModel->invalidate();
                m_impl->logView->horizontalHeader()->reset();
            });

    connect(m_impl->clearAction, &QAction::triggered, this, [=]() { m_impl->model->clear(); });

    m_impl->searchAction = new QWidgetAction(this);
    m_impl->searchButton = new QCtmToolButton(m_impl->searchEdit);
    m_impl->searchButton->setDefaultAction(m_impl->searchAction);
    m_impl->searchButton->setObjectName("searchAction");
    m_impl->searchAction->setDefaultWidget(m_impl->searchButton);
    m_impl->searchEdit->lineEdit()->addAction(m_impl->searchAction, QLineEdit::ActionPosition::TrailingPosition);
    m_impl->searchAction->setObjectName("searchAction");

    connect(m_impl->searchEdit->lineEdit(), &QLineEdit::returnPressed, this, [=]() { emit m_impl->searchAction->triggered(false); });

    connect(m_impl->searchAction,
            &QAction::triggered,
            this,
            [=](bool)
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

    connect(m_impl->copyAction,
            &QAction::triggered,
            this,
            [=]()
            {
                const auto& index = m_impl->logView->currentIndex();
                if (index.isValid())
                {
                    auto cb   = qApp->clipboard();
                    auto text = m_impl->model->data(m_impl->model->index(index.row(), 1), Qt::DisplayRole).toString() + " " +
                                m_impl->model->data(m_impl->model->index(index.row(), 2), Qt::DisplayRole).toString();
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
    \brief      更新日志数量.
*/
void QCtmLogWidget::updateLogCount()
{
    m_impl->errorAction->setText(tr("Error %1").arg(m_impl->model->errorCount()));
    m_impl->warningAction->setText(tr("Warn %1").arg(m_impl->model->warningCount()));
    m_impl->infoAction->setText(tr("Info %1").arg(m_impl->model->infoCount()));
}

/*!
    \brief      设置 Info 等级日志图标 \a icon.
    \sa         infoIcon()
*/
void QCtmLogWidget::setInfoIcon(const QIcon& icon)
{
    m_impl->model->setInfoIcon(icon);
    m_impl->infoAction->setIcon(icon);
}

/*!
    \brief      返回 Info 等级日志图标.
    \sa         setInfoIcon
*/
const QIcon& QCtmLogWidget::infoIcon() const { return m_impl->model->infoIcon(); }

/*!
    \brief      设置 Warning 日志图标 \a icon.
    \sa         warningIcon()
*/
void QCtmLogWidget::setWarningIcon(const QIcon& icon)
{
    m_impl->model->setWarningIcon(icon);
    m_impl->warningAction->setIcon(icon);
}

/*!
    \brief      返回 Warning 日志图标.
    \sa         setWarningIcon
*/
const QIcon& QCtmLogWidget::warningIcon() const { return m_impl->model->warningIcon(); }

/*!
    \brief      设置 Error 日志图标 \a icon.
    \sa         errorIcon()
*/
void QCtmLogWidget::setErrorIcon(const QIcon& icon)
{
    m_impl->model->setErrorIcon(icon);
    m_impl->errorAction->setIcon(icon);
}

/*!
    \brief      返回 Error 日志图标.
    \sa         setErrorIcon
*/
const QIcon& QCtmLogWidget::errorIcon() const { return m_impl->model->errorIcon(); }

/*!
    \brief      设置最大日志数量 \a count.
    \sa         maximumCount()
*/
void QCtmLogWidget::setMaximumCount(int count) { m_impl->model->setMaximumCount(count); }

/*!
    \brief      返回最大日志数量.
    \sa         setMaximumCount
*/
int QCtmLogWidget::maximumCount() const { return m_impl->model->maximumCount(); }
