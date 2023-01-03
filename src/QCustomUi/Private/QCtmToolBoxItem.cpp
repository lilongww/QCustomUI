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

#include "QCtmToolBoxItem_p.h"
#include "QCtmToolButton_p.h"
#include "QCtmWidgetItem_p.h"
#include "Util_p.h"

#include <QActionEvent>
#include <QApplication>
#include <QList>
#include <QPainter>
#include <QStyleOption>

struct QCtmToolBoxItem::Impl
{
    QList<QCtmWidgetItemPtr> items;
    QHBoxLayout* layout { nullptr };
    QWidget* m_content { nullptr };
    int stretch { 0 };
    QSize iconSize { 16, 16 };
};

QCtmToolBoxItem::QCtmToolBoxItem(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    ui.setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    m_impl->layout = new QHBoxLayout(ui.item_title);
    m_impl->layout->setAlignment(Qt::AlignRight);
    m_impl->layout->setContentsMargins(0, 0, 0, 0);
    m_impl->layout->setSpacing(0);
    connect(qApp,
            &QApplication::focusChanged,
            this,
            [=](auto, auto now)
            {
                auto setActive = [](bool act, QWidget* obj)
                {
                    obj->style()->unpolish(obj);
                    obj->setProperty("qcustomui_active", act);
                    obj->style()->polish(obj);
                };
                auto active = now && (now == this || isAncestorOf(now));
                setActive(active, ui.item_title);
                auto btns = ui.item_title->findChildren<QCtmToolButton*>();
                for (auto btn : btns)
                {
                    setActive(active, btn);
                    btn->setSelected(active);
                }
            });
}

QCtmToolBoxItem::~QCtmToolBoxItem() {}

void QCtmToolBoxItem::setTitle(const QString& text) { ui.item_title->setTitle(text); }

QString QCtmToolBoxItem::title() const { return ui.item_title->title(); }

void QCtmToolBoxItem::setContent(QWidget* widget)
{
    if (m_impl->m_content)
    {
        for (auto action : m_impl->m_content->actions())
        {
            removeAction(action);
        }
        delete m_impl->m_content;
    }
    ui.contentLayout->addWidget(widget);
    m_impl->m_content = widget;
    for (auto action : m_impl->m_content->actions())
    {
        addAction(action);
    }
    m_impl->m_content->installEventFilter(this);
}

QWidget* QCtmToolBoxItem::content() const { return m_impl->m_content; }

void QCtmToolBoxItem::setStretch(int stretch) { m_impl->stretch = stretch; }

int QCtmToolBoxItem::stretch() const { return m_impl->stretch; }

void QCtmToolBoxItem::setIcon(const QIcon& icon) { ui.item_title->setIcon(icon); }

const QIcon& QCtmToolBoxItem::icon() const { return ui.item_title->icon(); }

void QCtmToolBoxItem::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(size);
}

const QSize& QCtmToolBoxItem::iconSize() const { return m_impl->iconSize; }

void QCtmToolBoxItem::actionEvent(QActionEvent* event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCtmToolBoxItem::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
        Util::addItem(item, m_impl->items, event->before(), m_impl->layout);
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->items, m_impl->layout);
    }
}

bool QCtmToolBoxItem::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->m_content)
    {
        if (event->type() == QEvent::ActionAdded)
        {
            auto evt = dynamic_cast<QActionEvent*>(event);
            if (evt)
                insertAction(evt->before(), evt->action());
        }
        else if (event->type() == QEvent::ActionRemoved)
        {
            auto evt = dynamic_cast<QActionEvent*>(event);
            if (evt)
            {
                removeAction(evt->action());
            }
        }
    }
    return false;
}
