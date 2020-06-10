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

#include "QCtmNavigationSideBar.h"
#include "Private/QCtmActionGroup_p.h"
#include "Private/QCtmWidgetItem_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QToolButton>
#include <QVBoxLayout>
#include <QActionEvent>

static Q_CONSTEXPR const char* ActionPosProperty = "QCtm-ActionPosition";

struct QCtmNavigationSideBar::Impl
{
	QVBoxLayout* layout{ nullptr };
	QVBoxLayout* topLayout{ nullptr };
	QVBoxLayout* bottomLayout{ nullptr };
	QSize iconSize{ 25,25 };
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
        for (auto &item : items)
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

QCtmNavigationSideBar::QCtmNavigationSideBar(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->layout = new QVBoxLayout(this);
	m_impl->layout->setContentsMargins(0, 10, 0, 10);
	m_impl->topLayout = new QVBoxLayout;
	m_impl->layout->addLayout(m_impl->topLayout);
	m_impl->topLayout->setMargin(0);
	m_impl->topLayout->setSpacing(25);
	m_impl->bottomLayout = new QVBoxLayout;
	m_impl->bottomLayout->setMargin(0);
	m_impl->bottomLayout->setSpacing(25);
	m_impl->layout->addStretch(1);
	m_impl->layout->addLayout(m_impl->bottomLayout);

	m_impl->actionGroup = new QCtmActionGroup(this);
	setFixedWidth(50);
}

QCtmNavigationSideBar::~QCtmNavigationSideBar()
{
}

/**
 * @brief       添加一个侧边栏按钮
 * @param[in]   icon 按钮图标
 * @param[in]   text 按钮文字，用于显示tooltip
 * @param[in]   pos 停靠方向
 * @Return:     侧边栏按钮地址
 */
QAction* QCtmNavigationSideBar::addAction(const QIcon& icon, const QString& text, ActionPosition pos)
{
	auto count = this->count(pos);
	return insertAction(count, icon, text, pos);
}

/**
 * @brief		添加一个action
 * @param[in]	action 要添加的action
 * @param[in]	pos action停靠方向
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

/**
 * @brief       获取按钮数量
 * @param[in]   pos 停靠方向
 * @Return:     按钮数量
 */
int QCtmNavigationSideBar::count(ActionPosition align) const
{
	return align == ActionPosition::Top ? m_impl->topActions.size() : m_impl->bottomActions.size();
}

/**
 * @brief       设置图标显示大小
 * @param[in]   size 图标的大小
 */
void QCtmNavigationSideBar::setIconSize(const QSize& size)
{
	m_impl->iconSize = size;
}

/**
 * @brief       获取图标显示大小
 * @Return:     图标显示大小
 */
const QSize& QCtmNavigationSideBar::iconSize() const
{
	return m_impl->iconSize;
}

/**
 * @brief		查找指定位置的action
 * @param[in]	index 指定位置
 * @param[in]	pos action停靠方向
 * @Return:		action action地址
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

void QCtmNavigationSideBar::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCtmNavigationSideBar::actionEvent(QActionEvent *event)
{
    ActionPosition pos = Top;
    if (event->action()->property(ActionPosProperty).isValid())
    {
        pos = (ActionPosition)event->action()->property(ActionPosProperty).toInt();
    }
    if (event->type() == QEvent::ActionAdded)
    {
        auto addItem = [](QCtmWidgetItemPtr item, QList<QCtmWidgetItemPtr>& items, QAction* before, QBoxLayout* layout) {
            auto it = std::find_if(items.begin(), items.end(), [=](QCtmWidgetItemPtr item) { return item->action() == before; });
            it = items.insert(it, item);
            auto index = items.indexOf(*it);
            layout->insertWidget(index, item->widget());
            if (item->isCustomWidget())
            {
                item->widget()->show();
            }
        };
        QCtmWidgetItemPtr item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Vertical, this);
        if (!item->isCustomWidget())
        {
            auto btn = qobject_cast<QToolButton*>(item->widget());
            if (btn)
            {
                btn->setIconSize(m_impl->iconSize);
                connect(this, &QCtmNavigationSideBar::iconSizeChanged, btn, &QToolButton::setIconSize);
            }
        }
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

/**
 * @brief		插入一个action
 * @param[in]	index 插入位置
 * @param[in]	action 要插入的action
 * @param[in]   pos action停靠方向
 */
void QCtmNavigationSideBar::insertAction(int index, QAction* action, ActionPosition pos)
{
    auto before = actionAt(index, pos);
    action->setProperty(ActionPosProperty, pos);
    QWidget::insertAction(before, action);
}

/**
 * @brief       插入一个侧边栏按钮
 * @param[in]   index 侧边栏按钮位置
 * @param[in]   icon 按钮图标
 * @param[in]   text 按钮文字，用于显示tooltip
 * @param[in]   pos 停靠方向
 */
QAction* QCtmNavigationSideBar::insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto action = new QAction(icon, "", nullptr);
	action->setToolTip(text);
	insertAction(index, action, pos);
	return action;
}

/**
 * @brief       获取侧边栏按钮位置，如果action不存在则返回-1
 * @param[in]   action 侧边栏按钮
 * @Return:     侧边栏按钮位置
 */
int QCtmNavigationSideBar::indexOf(QAction* action) const
{
	if (m_impl->bottomActions.contains(m_impl->find(action)))
		return m_impl->bottomActions.indexOf(m_impl->find(action));
	else if (m_impl->topActions.contains(m_impl->find(action)))
		return m_impl->topActions.contains(m_impl->find(action));
	return -1;
}

