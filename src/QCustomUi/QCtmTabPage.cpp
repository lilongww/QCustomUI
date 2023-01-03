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

#include "QCtmTabPage.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"
#include "QCtmTabWidget.h"

#include <QActionEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>

#include <assert.h>

struct QCtmTabPage::Impl
{
    QList<QCtmWidgetItemPtr> actions;
    QCtmTabWidget* parent { nullptr };
    QWidget* cornerWidget { nullptr };
    QHBoxLayout* cornerLayout { nullptr };
    QHBoxLayout* layout { nullptr };
    QWidget* centralWidget { nullptr };
    QSize iconSize { 16, 16 };
};

/*!
    \class      QCtmTabPage
    \brief      QCtmTabWidget 的子页面.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmTabPage.h
*/

/*!
    \fn         void QCtmTabPage::iconSizeChanged(const QSize& size);
    \brief      当Action的图标大小发生改变时发送该信号 \a size.
    \sa         setIconSize
*/

/*!
    \brief      添加图标为 \a icon 文本为 \a text 的Action到角落窗口上.
    \sa         insertAction
*/
QAction* QCtmTabPage::addAction(const QIcon& icon, const QString& text) { return insertAction(count(), icon, text); }

/*!
    \overload   addAction
                添加文本为 \a text 的Action到角落窗口上.
    \sa         QCtmTabPage::addAction
*/
QAction* QCtmTabPage::addAction(const QString& text) { return addAction(QIcon(), text); }

/*!
    \overload   insertAction
                添加图标为 \a icon 文本为 \a text 的Action，并插入到角落窗口的 \a index 位置.
    \sa         QCtmTabPage::insertAction
*/
QAction* QCtmTabPage::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->cornerWidget);
    insertAction(index, action);
    return action;
}

/*!
    \overload   insertAction
                插入 \a action 到角落窗口的 \a index 位置.
    \sa         QWidget::insertAction
*/
void QCtmTabPage::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

/*!
    \brief      添加文本为 \a text 的Action，并插入到角落窗口的 \a index 位置.
*/
QAction* QCtmTabPage::insertAction(int index, const QString& text) { return insertAction(index, QIcon(), text); }

/*!
    \brief      返回序号为 \a index 的Action.
    \sa         addAction, insertAction
*/
QAction* QCtmTabPage::actionAt(int index) const
{
    if (index < 0)
        return nullptr;
    if (m_impl->actions.size() > index)
    {
        return m_impl->actions.at(index)->action();
    }
    return nullptr;
}

/*!
    \brief      取出中央窗口，不再控制其生命周期.
    \sa         setCentralWidget, centralWidget()
*/
QWidget* QCtmTabPage::takeCentralWidget() const
{
    if (m_impl->layout->count() > 0)
    {
        auto item = m_impl->layout->takeAt(0);
        if (item->widget())
        {
            item->widget()->setParent(0);
            item->widget()->hide();
            return item->widget();
        }
    }
    return nullptr;
}

/*!
    \brief      返回中央窗口.
    \sa         setCentralWidget
*/
QWidget* QCtmTabPage::centralWidget() const { return m_impl->centralWidget; }

/*!
    \brief      设置Action图标大小 \a size.
    \sa         iconSize
*/
void QCtmTabPage::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(size);
}

/*!
    \brief      返回Action的图标大小.
    \sa         setIconSize
*/
const QSize& QCtmTabPage::iconSize() const { return m_impl->iconSize; }
/*!
    \brief      返回Action的数量.
    \sa         addAction, insertAction
*/
int QCtmTabPage::count() const { return m_impl->actions.size(); }

/*!
    \reimp
*/
void QCtmTabPage::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/*!
    \reimp
*/
void QCtmTabPage::actionEvent(QActionEvent* event)
{
    if (event->type() == QActionEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCtmTabPage::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
        Util::addItem(item, m_impl->actions, event->before(), m_impl->cornerLayout);
    }
    else if (event->type() == QActionEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->actions, m_impl->cornerLayout);
    }
}

/*!
    \brief      返回角落窗口.
*/
QWidget* QCtmTabPage::cornerWidget() const { return m_impl->cornerWidget; }

/*!
    \brief      设置中央窗口 \a widget.
    \sa         centralWidget
*/
void QCtmTabPage::setCentralWidget(QWidget* widget)
{
    m_impl->centralWidget = widget;
    m_impl->layout->addWidget(widget);
    widget->show();
}

/*!
    \brief      构造函数 \a parent.
*/
QCtmTabPage::QCtmTabPage(QCtmTabWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    assert(parent);
    m_impl->cornerWidget = new QWidget;
    m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
    m_impl->cornerLayout->setContentsMargins(0, 0, 0, 0);

    m_impl->layout = new QHBoxLayout(this);
}

/*!
    \brief      析构函数.
*/
QCtmTabPage::~QCtmTabPage()
{
    m_impl->actions.clear();
    delete m_impl->cornerWidget;
}
