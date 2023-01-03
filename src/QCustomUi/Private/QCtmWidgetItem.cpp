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

#include "QCtmSeparator_p.h"
#include "QCtmToolButton_p.h"
#include "QCtmWidgetItem_p.h"

#include <QWidgetAction>

inline void setToolButtonProperty(QAction* action, QCtmToolButton* btn)
{
    auto setToolButtonStyle = [](QAction* action, QCtmToolButton* btn)
    {
        if (action->icon().isNull())
            btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        else if (action->text().isEmpty())
            btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        else
            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    };
    btn->setDefaultAction(action);
    btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    btn->setObjectName(action->objectName());
    if (btn->parent())
        btn->setVisible(action->isVisible());
    setToolButtonStyle(action, btn);
    QObject::connect(action,
                     &QAction::changed,
                     btn,
                     [=]()
                     {
                         setToolButtonStyle(action, btn);
                         if (btn->parent())
                             btn->setVisible(action->isVisible());
                     });
    QObject::connect(action, &QObject::objectNameChanged, btn, [=](const QString& name) { btn->setObjectName(name); });
};

inline void setWidgetProperty(QAction* action, QWidget* widget)
{
    QObject::connect(action,
                     &QAction::changed,
                     widget,
                     [=]()
                     {
                         if (widget->parent())
                             widget->setVisible(action->isVisible());
                     });
    QObject::connect(action, &QObject::objectNameChanged, widget, [=](const QString& name) { widget->setObjectName(name); });
}

struct QCtmWidgetItem::Impl
{
    QAction* action { nullptr };
    QWidget* widget { nullptr };
    bool customWidget { false };
};

QCtmWidgetItem::QCtmWidgetItem(QAction* action, Qt::Orientation orientation, const QSize& iconSize, QWidget* parent)
    : QObject(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->action = action;

    auto wa = qobject_cast<QWidgetAction*>(action);
    if (wa && wa->defaultWidget())
    {
        m_impl->customWidget = true;
        m_impl->widget       = wa->defaultWidget();
        if (auto btn = qobject_cast<QCtmToolButton*>(m_impl->widget); btn)
        {
            setToolButtonProperty(action, btn);
            btn->setIconSize(iconSize);
            connect(this, &QCtmWidgetItem::iconSizeChanged, btn, &QCtmToolButton::setIconSize);
        }
        else
            setWidgetProperty(action, m_impl->widget);
    }
    else
    {
        if (action->isSeparator())
        {
            m_impl->widget = new QCtmSeparator(orientation, parent);
            m_impl->widget->setObjectName(action->objectName());
            QObject::connect(
                action, &QObject::objectNameChanged, m_impl->widget, [=](const QString& name) { m_impl->widget->setObjectName(name); });
        }
        else
        {
            auto btn = new QCtmToolButton(parent);
            setToolButtonProperty(action, btn);
            m_impl->widget = btn;
            btn->setIconSize(iconSize);
            connect(this, &QCtmWidgetItem::iconSizeChanged, btn, &QCtmToolButton::setIconSize);
        }
    }
}

QCtmWidgetItem::~QCtmWidgetItem()
{
    if (!m_impl->customWidget && m_impl->widget)
        delete m_impl->widget;
}

void QCtmWidgetItem::release()
{
    m_impl->action = nullptr;
    m_impl->widget = nullptr;
}

QAction* QCtmWidgetItem::action() const { return m_impl->action; }

QWidget* QCtmWidgetItem::widget() const { return m_impl->widget; }

bool QCtmWidgetItem::isCustomWidget() const { return m_impl->customWidget; }
