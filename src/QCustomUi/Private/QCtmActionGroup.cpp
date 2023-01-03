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

#include "QCtmActionGroup_p.h"

#include <QSet>

struct QCtmActionGroup::Impl
{
    QSet<QAction*> actions;
    QAction* current { nullptr };
};

QCtmActionGroup::QCtmActionGroup(QObject* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {}

QCtmActionGroup::~QCtmActionGroup() {}

void QCtmActionGroup::addAction(QAction* action)
{
    m_impl->actions.insert(action);
    connect(action, &QAction::changed, this, &QCtmActionGroup::actionChanged);
}

void QCtmActionGroup::removeAction(QAction* action) { m_impl->actions.remove(action); }

void QCtmActionGroup::actionChanged()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action->isChecked())
    {
        if (action != m_impl->current)
        {
            if (m_impl->current)
                m_impl->current->setChecked(false);
            m_impl->current = action;
        }
    }
    else if (action == m_impl->current)
    {
        m_impl->current = 0;
    }
}
