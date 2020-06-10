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

#include "QCtmTabPage.h"
#include "QCtmTabWidget.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QActionEvent>

#include <assert.h>

struct QCtmTabPage::Impl
{
	QList<QCtmWidgetItemPtr> actions;
	QCtmTabWidget* parent{ nullptr };
	QWidget* cornerWidget{ nullptr };
	QHBoxLayout* cornerLayout{ nullptr };
	QHBoxLayout* layout{ nullptr };
	QWidget* centralWidget{ nullptr };
};

/**
 * @brief  		右上角添加一个action
 * @param[in]  	icon 图标
 * @param[in] 	text 文字
 * @Return:   	action 地址
 */
QAction* QCtmTabPage::addAction(const QIcon& icon, const QString& text)
{
	return insertAction(count(), icon, text);
}

/**
 * @brief  		右上角添加一个action
 * @param[in]  	text 文字
 * @Return:   	action 地址
 */
QAction* QCtmTabPage::addAction(const QString& text)
{
	return addAction(QIcon(), text);
}

/**
 * @brief  		右上角插入一个action
 * @param[in]  	index 插入位置
 * @param[in] 	icon 图标
 * @param[in] 	text 文字
 * @Return:   	action 地址
 */
QAction* QCtmTabPage::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->cornerWidget);
	insertAction(index, action);
	return action;
}

/**
 * @brief		插入action
 * @param[in]	index 要插入的位置
 * @param[in]	action 要插入的action
 */
void QCtmTabPage::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

/**
 * @brief  		右上角插入一个action
 * @param[in]  	index 插入位置
 * @param[in] 	text 文字
 * @Return:   	action 地址
 */
QAction* QCtmTabPage::insertAction(int index, const QString& text)
{
	return insertAction(index, QIcon(), text);
}

/**
 * @brief		获取指定位置的action
 * @param[in]	index 指定位置
 * @Return:		action地址
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

/**
 * @brief		释放当前中央窗口
 * @Return:		当前中央窗口地址
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

/**
 * @brief		中央窗口地址
 */
QWidget* QCtmTabPage::centralWidget() const
{
	return m_impl->centralWidget;
}

/**
 * @brief      获取action数量
 * @Return:    action数量
 */
int QCtmTabPage::count() const
{
	return m_impl->actions.size();
}

void QCtmTabPage::paintEvent([[maybe_unused]] QPaintEvent *event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCtmTabPage::actionEvent(QActionEvent* event)
{
    if (event->type() == QActionEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
        Util::addItem(item, m_impl->actions, event->before(), m_impl->cornerLayout);
    }
    else if (event->type() == QActionEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->actions, m_impl->cornerLayout);
    }
}

QWidget* QCtmTabPage::cornerWidget() const
{
	return m_impl->cornerWidget;
}

void QCtmTabPage::setCentralWidget(QWidget* widget)
{
	m_impl->centralWidget = widget;
	m_impl->layout->addWidget(widget);
	widget->show();
}

QCtmTabPage::QCtmTabPage(QCtmTabWidget* parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	assert(parent);
	m_impl->cornerWidget = new QWidget;
	m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
	m_impl->cornerLayout->setMargin(0);

	m_impl->layout = new QHBoxLayout(this);
}

QCtmTabPage::~QCtmTabPage()
{
	m_impl->actions.clear();
	delete m_impl->cornerWidget;
}
