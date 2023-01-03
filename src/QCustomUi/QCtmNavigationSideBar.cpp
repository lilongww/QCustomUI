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

#include "QCtmNavigationSideBar.h"
#include "Private/QCtmActionGroup_p.h"
#include "Private/QCtmWidgetItem_p.h"

#include <QActionEvent>
#include <QPainter>
#include <QStyleOption>
#include <QToolButton>
#include <QVBoxLayout>

static Q_CONSTEXPR const char* ActionPosProperty = "QCtm-ActionPosition";

struct QCtmNavigationSideBar::Impl
{
    QVBoxLayout* layout { nullptr };
    QVBoxLayout* topLayout { nullptr };
    QVBoxLayout* bottomLayout { nullptr };
    QSize iconSize { 25, 25 };
    QList<QCtmWidgetItemPtr> topActions;
    QList<QCtmWidgetItemPtr> bottomActions;

    QCtmActionGroup* actionGroup;

    QCtmWidgetItemPtr find(QAction* action)
    {
        auto item = find(action, Top);
        return item ? item : find(action, Bottom);
    }

    QCtmWidgetItemPtr find(QAction* action, const QList<QCtmWidgetItemPtr>& items)
    {
        for (auto& item : items)
        {
            if (item->action() == action)
                return item;
        }
        return nullptr;
    }

    QCtmWidgetItemPtr find(QAction* action, ActionPosition pos)
    {
        switch (pos)
        {
        case Top:
            return find(action, topActions);
        case Bottom:
            return find(action, topActions);
        }
        return nullptr;
    }
};

/*!
    \class      QCtmNavigationSideBar
    \brief      导航侧边栏，类似一个ToolBox.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmNavigationSideBar.h
*/

/*!
    \property   QCtmNavigationSideBar::iconSize
    \brief      该属性决定侧边栏的图标大小.
*/

/*!
    \enum       QCtmNavigationSideBar::ActionPosition
                Action的安放位置.
    \value      Top
                Action安放在侧边栏的顶部.
    \value      Bottom
                Action安放在侧边栏的底部.
*/

/*!
    \fn         void QCtmNavigationSideBar::iconSizeChanged(const QSize& size)
    \brief      图标大小 \a size 发生改变时发送该信号.
*/

/*!
    \brief      构造一个父窗口为 \a parent 的导航侧边栏对象.
*/
QCtmNavigationSideBar::QCtmNavigationSideBar(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setContentsMargins(0, 10, 0, 10);
    m_impl->topLayout = new QVBoxLayout;
    m_impl->layout->addLayout(m_impl->topLayout);
    m_impl->topLayout->setContentsMargins(0, 0, 0, 0);
    m_impl->topLayout->setSpacing(25);
    m_impl->bottomLayout = new QVBoxLayout;
    m_impl->bottomLayout->setContentsMargins(0, 0, 0, 0);
    m_impl->bottomLayout->setSpacing(25);
    m_impl->layout->addStretch(1);
    m_impl->layout->addLayout(m_impl->bottomLayout);

    m_impl->actionGroup = new QCtmActionGroup(this);
    setFixedWidth(50);
    setAttribute(Qt::WA_StyledBackground);
}

/*!
    \brief      销毁该侧边栏对象.
*/
QCtmNavigationSideBar::~QCtmNavigationSideBar() {}

/*!
    \overload   addAction
                重载函数，在侧边栏的 \a pos 位置添加一个图标为 \a icon 文本为 \a text 的Action.
    \sa         QWidget::addAction
*/
QAction* QCtmNavigationSideBar::addAction(const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto count = this->count(pos);
    return insertAction(count, icon, text, pos);
}

/*!
    \overload   addAction
                重载函数，在侧边栏的 \a pos 添加一个 \a action.
    \sa         QWidget::addAction
*/
void QCtmNavigationSideBar::addAction(QAction* action, ActionPosition pos)
{
    int index = 0;
    if (pos == ActionPosition::Bottom)
        index = m_impl->bottomActions.size();
    else
        index = m_impl->topActions.size();
    insertAction(index, action, pos);
}

/*!
    \brief      返回 \a pos 位置的Action数量.
    \sa         addAction, insertAction
*/
int QCtmNavigationSideBar::count(ActionPosition pos) const
{
    return pos == ActionPosition::Top ? m_impl->topActions.size() : m_impl->bottomActions.size();
}

/*!
    \brief      设置Action的图标大小 \a size.
    \sa         iconSize
*/
void QCtmNavigationSideBar::setIconSize(const QSize& size) { m_impl->iconSize = size; }

/*!
    \brief      返回Action的图标大小.
    \sa         setIconSize
*/
const QSize& QCtmNavigationSideBar::iconSize() const { return m_impl->iconSize; }

/*!
    \brief      返回位于 \a pos \a index 的Action.
    \sa         addAction, insertAction, indexOf
*/
QAction* QCtmNavigationSideBar::actionAt(int index, ActionPosition pos) const
{
    if (index < 0)
        return nullptr;
    switch (pos)
    {
    case QCtmNavigationSideBar::Top:
        if (m_impl->topActions.size() > index)
            return m_impl->topActions.at(index)->action();
        else
            return nullptr;
    case QCtmNavigationSideBar::Bottom:
        if (m_impl->bottomActions.size() > index)
            return m_impl->bottomActions.at(index)->action();
        else
            return nullptr;
    default:
        break;
    }
    return nullptr;
}

/*!
    \reimp
*/
void QCtmNavigationSideBar::actionEvent(QActionEvent* event)
{
    ActionPosition pos = Top;
    if (event->action()->property(ActionPosProperty).isValid())
    {
        pos = static_cast<ActionPosition>(event->action()->property(ActionPosProperty).toInt());
    }
    if (event->type() == QEvent::ActionAdded)
    {
        auto addItem = [](QCtmWidgetItemPtr item, QList<QCtmWidgetItemPtr>& items, QAction* before, QBoxLayout* layout)
        {
            auto it    = std::find_if(items.begin(), items.end(), [=](QCtmWidgetItemPtr item) { return item->action() == before; });
            it         = items.insert(it, item);
            auto index = items.indexOf(*it);
            layout->insertWidget(index, item->widget());
            if (item->isCustomWidget())
            {
                item->widget()->show();
            }
        };
        QCtmWidgetItemPtr item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Vertical, m_impl->iconSize, this);
        connect(this, &QCtmNavigationSideBar::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
        switch (pos)
        {
        case Top:
            addItem(item, m_impl->topActions, event->before(), m_impl->topLayout);
            break;
        case Bottom:
            addItem(item, m_impl->bottomActions, event->before(), m_impl->bottomLayout);
            break;
        }
        m_impl->actionGroup->addAction(item->action());
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        auto item = m_impl->find(event->action(), pos);
        if (item->isCustomWidget())
        {
            item->widget()->hide();
        }
        m_impl->actionGroup->removeAction(event->action());
        switch (pos)
        {
        case Top:
            m_impl->topLayout->takeAt(m_impl->topLayout->indexOf(item->widget()));
            m_impl->topActions.removeOne(item);
            break;
        case Bottom:
            m_impl->bottomLayout->takeAt(m_impl->bottomLayout->indexOf(item->widget()));
            m_impl->bottomActions.removeOne(item);
            break;
        }
    }
}

/*!
    \overload   insertAction
                重载函数，在 \a pos \a index 位置插入一个 \a action.
    \sa         QWidget::insertAction
*/
void QCtmNavigationSideBar::insertAction(int index, QAction* action, ActionPosition pos)
{
    action->setProperty(ActionPosProperty, pos);
    QWidget::insertAction(actionAt(index, pos), action);
}

/*!
    \overload   insertAction
                重载函数， 在 \a pos \a index 位置插入并返回一个图标为 \a icon 文本为 \a text 的Action.
    \sa         QWidget::insertAction
*/
QAction* QCtmNavigationSideBar::insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto action = new QAction(icon, "", nullptr);
    action->setToolTip(text);
    insertAction(index, action, pos);
    return action;
}

/*!
    \brief      返回 \a action 的序号.
    \sa         count, actionAt
*/
int QCtmNavigationSideBar::indexOf(QAction* action) const
{
    if (m_impl->bottomActions.contains(m_impl->find(action)))
        return m_impl->bottomActions.indexOf(m_impl->find(action));
    else if (m_impl->topActions.contains(m_impl->find(action)))
        return m_impl->topActions.contains(m_impl->find(action));
    return -1;
}
