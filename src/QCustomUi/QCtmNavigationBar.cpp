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

#include "QCtmNavigationBar.h"
#include "QCtmNavigationSidePane.h"
#include "QCtmMessageTip.h"
#include "Private/QCtmNavigationImageButton_p.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"
#include "Private/QCtmUserButton_p.h"

#include <QAction>
#include <QWidgetAction>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QMenu>
#include <QToolButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QActionEvent>

static Q_CONSTEXPR const char* ActionPosProperty = "QCtm_ActionPosition";

struct QCtmNavigationBar::Impl
{
	QList<QCtmWidgetItemPtr> leftItems;
	QList<QCtmWidgetItemPtr> rightItems;

    QHBoxLayout* leftLayout;
    QHBoxLayout* rightLayout;

    QCtmWidgetItemPtr find(QAction* action)
    {
        auto item = find(action, Left);
        return item ? item : find(action, Right);
    }

    QCtmWidgetItemPtr find(QAction* action, const QList<QCtmWidgetItemPtr>& items)
    {
        for (const auto &item : items)
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
        case Left:
            return find(action, leftItems);
        case Right:
            return find(action, rightItems);
        }
        return nullptr;
    }

    QCtmWidgetItemPtr find(QWidget* widget, const QList<QCtmWidgetItemPtr>& items)
    {
        for (const auto &item : items)
        {
            if (item->widget() == widget)
                return item;
        }
    }

    QCtmWidgetItemPtr find(QWidget* widget, ActionPosition pos)
    {
        switch (pos)
        {
        case Left:
            return find(widget, leftItems);
        case Right:
            return find(widget, rightItems);
        }
        return nullptr;
    }
};

QCtmNavigationBar::QCtmNavigationBar(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	setFixedHeight(50);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(1);
    layout->setSpacing(0);
    m_impl->leftLayout = new QHBoxLayout;
    m_impl->leftLayout->setMargin(0);
    m_impl->leftLayout->setSpacing(1);
    m_impl->rightLayout = new QHBoxLayout;
    m_impl->rightLayout->setMargin(0);
    m_impl->rightLayout->setSpacing(1);

    layout->addLayout(m_impl->leftLayout);
    layout->addStretch(1);
    layout->addLayout(m_impl->rightLayout);
}

QCtmNavigationBar::~QCtmNavigationBar()
{
}

/**
 * @brief       Add a action.
 */
QAction* QCtmNavigationBar::addAction(const QString& text, ActionPosition pos)
{
	return addAction(QIcon(), text, pos);
}

/**
 * @brief  	    Add a action.
 */
QAction* QCtmNavigationBar::addAction(const QIcon& icon, const QString& text, ActionPosition pos)
{
    return insertAction(count(pos), icon, text, pos);
}

/**
 * @brief		Insert a action.
 */
void QCtmNavigationBar::insertAction(int index, QAction* action, ActionPosition pos)
{
    auto before = actionAt(index, pos);
    action->setProperty(ActionPosProperty, pos);
    QWidget::insertAction(before, action);
}

/**
 * @brief		Add a action.
 */
void QCtmNavigationBar::addAction(QAction* action, ActionPosition pos)
{
	int index = pos == ActionPosition::Right ? m_impl->rightItems.size() : m_impl->leftItems.size();
	insertAction(index, action, pos);
}

void QCtmNavigationBar::paintEvent([[maybe_unused]] QPaintEvent* e)
{
	QPainter p(this);
	drawBackground(&p);
}

void QCtmNavigationBar::actionEvent(QActionEvent *event)
{
    ActionPosition pos = Left;
    if (event->action()->property(ActionPosProperty).isValid())
    {
        pos = static_cast<ActionPosition>(event->action()->property(ActionPosProperty).toInt());
    }

    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
        switch (pos)
        {
        case Left:
            Util::addItem(item, m_impl->leftItems, event->before(), m_impl->leftLayout);
            break;
        case Right:
            Util::addItem(item, m_impl->rightItems, event->before(), m_impl->rightLayout);
            break;
        }
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        auto item = m_impl->find(event->action(), pos);
        if (item->isCustomWidget())
        {
            item->widget()->hide();
            item->widget()->setParent(nullptr);
        }
        switch (pos)
        {
        case Left:
            m_impl->leftLayout->takeAt(m_impl->leftLayout->indexOf(item->widget()));
            m_impl->leftItems.removeOne(item);
            break;
        case Right:
            m_impl->rightLayout->takeAt(m_impl->rightLayout->indexOf(item->widget()));
            m_impl->rightItems.removeOne(item);
            break;
        }
    }
}

void QCtmNavigationBar::drawBackground(QPainter *p)
{
	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, p, this);
}

/**
 * @brief       Add a separator.
 */
QAction* QCtmNavigationBar::addSeparator(ActionPosition pos)
{
	return insertSeparator(count(pos), pos);
}

/**
 * @brief  	    Insert a separator.
 */
QAction* QCtmNavigationBar::insertSeparator(int index, ActionPosition pos)
{
    auto action = new QAction(nullptr);
    action->setSeparator(true);
    insertAction(index, action, pos);
	return action;
}

/**
 * @brief  	    Add a pane.
 */
QAction* QCtmNavigationBar::addPane(const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane)
{
	auto count = this->count(pos);
	auto action = insertPane(count, icon, text, pos, pane);
	return action;
}

/**
 * @brief  	    Add a pane.
 */
QAction* QCtmNavigationBar::addPane(const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane)
{
	return addPane(QIcon(), text, pos, pane);
}

/**
 * @brief		Add a pane.
 */
void QCtmNavigationBar::addPane(QAction* action, ActionPosition pos, QCtmNavigationSidePane* pane)
{
    insertPane(count(pos), action, pos, pane);
}

/**
 * @brief  	    Insert a action.
 */
QAction* QCtmNavigationBar::insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto action = new QAction(icon, text, nullptr);
	insertAction(index, action, pos);
	return action;
}

/**
 * @brief  	    Add a action.
 */
QAction* QCtmNavigationBar::insertAction(int index, const QString& text, ActionPosition pos)
{
	return insertAction(index, QIcon(), text, pos);
}

/**
 * @brief  	    Insert a pane.
 */
QAction* QCtmNavigationBar::insertPane(int index, const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane)
{
	auto action = insertAction(index, icon, text, pos);
    insertPane(index, action, pos, pane);
	return action;
}

/**
 * @brief  	    Insert a pane.
 */
QAction* QCtmNavigationBar::insertPane(int index, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane)
{
	return insertPane(index, QIcon(), text, pos, pane);
}

/**
 * @brief  	    Insert a pane.
 */
void QCtmNavigationBar::insertPane(int index, QAction* action, ActionPosition pos, QCtmNavigationSidePane* pane)
{
    if (!this->actions().contains(action))
        insertAction(index, action, pos);
    action->setCheckable(true);
    if (pane->navigationBar() != this)
    {
        pane->setNavigationBar(this);
    }
    pane->bindAction(action);
    pane->hide();

    connect(action, &QAction::toggled, this, [=](bool checked)
    {
        if (checked)
            pane->show();
        else
            pane->hide();
    });

    connect(pane, &QCtmNavigationSidePane::paneClosed, this, [=]() { action->setChecked(false); }, Qt::QueuedConnection);
}

/**
 * @brief  	    Get the index of the action.
 */
int QCtmNavigationBar::indexOf(QAction* action) const
{
    auto item = m_impl->find(action, Left);
    if (item)
        return m_impl->leftItems.indexOf(item);
    else
    {
        item = m_impl->find(action, Right);
        if (item)
            return m_impl->rightItems.indexOf(item);
    }
	return -1;
}

/**
 * @brief  	    Get the count of the pos.
 */
int QCtmNavigationBar::count(ActionPosition pos) const
{
	switch (pos)
	{
	case ActionPosition::Left:
		return m_impl->leftItems.size();
	case ActionPosition::Right:
		return m_impl->rightItems.size();
	}
	return 0;
}

/**
 * @brief  	    Get the rect of the action.
 */
QRect QCtmNavigationBar::actionRect(QAction* action)
{
    auto item = m_impl->find(action);
	if (!item)
		return QRect();
	return item->widget()->geometry();
}

/**
 * @brief  	    Add a help action.
 */
QAction* QCtmNavigationBar::addHelp(const QUrl& filePath, const QIcon& icon, ActionPosition pos)
{
	auto index = count(pos);
	return insertHelp(index, filePath, icon, pos);
}

/**
 * @brief  	    Insert a help action.
 */
QAction* QCtmNavigationBar::insertHelp(int index, const QUrl& filePath, const QIcon& icon, ActionPosition pos /*= Right*/)
{
	auto help = new QAction(icon, "", this);
	connect(help, &QAction::triggered, this, [=]() {
		QDesktopServices::openUrl(filePath);
	});
	insertAction(index, help, pos);
	return help;
}

/**
 * @brief		Add a logo action.
 */
QAction* QCtmNavigationBar::addLogo(const QIcon& icon, ActionPosition pos)
{
	auto index = count(pos);
	return insertLogo(index, icon, pos);
}

/**
 * @brief		Insert a logo action.
 */
QAction* QCtmNavigationBar::insertLogo(int index, const QIcon& icon, ActionPosition pos /*= Left*/)
{
	QWidgetAction* action = new QWidgetAction(this);
	action->setIcon(icon);
	QCtmNavigationImageButton* imageBtn = new QCtmNavigationImageButton(this);
	imageBtn->setIcon(icon);
	imageBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	action->setDefaultWidget(imageBtn);
	imageBtn->setDefaultAction(action);

	QObject::connect(action, &QAction::objectNameChanged, imageBtn, &QCtmNavigationImageButton::setObjectName);
	insertAction(index, action, pos);
	return action;
}

/**
 * @brief		Add a user action.
 */
QAction* QCtmNavigationBar::addUser(const QIcon& icon, const QString& text, ActionPosition pos)
{
	return insertUser(count(pos), icon, text, pos);
}

/**
 * @brief		Insert a user action.
 */
QAction* QCtmNavigationBar::insertUser(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
	QWidgetAction *action = new QWidgetAction(this);
	QCtmUserButton* btn = new QCtmUserButton;
	action->setDefaultWidget(btn);
	btn->setDefaultAction(action);

	action->setText(text);
	action->setIcon(icon);
	btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	connect(action, &QAction::objectNameChanged, btn, &QObject::setObjectName);
	insertAction(index, action, pos);
	return action;
}

/**
 * @brief		Get the action at index and pos.
 */
QAction* QCtmNavigationBar::actionAt(int index, ActionPosition pos) const
{
    if (index < 0)
        return nullptr;
    switch (pos)
    {
    case QCtmNavigationBar::Left:
        if (m_impl->leftItems.size() > index)
            return m_impl->leftItems.at(index)->action();
        else
            return nullptr;
    case QCtmNavigationBar::Right:
        if (m_impl->rightItems.size() > index)
            return m_impl->rightItems.at(index)->action();
        else
            return nullptr;
    default:
        break;
    }
    return nullptr;
}
