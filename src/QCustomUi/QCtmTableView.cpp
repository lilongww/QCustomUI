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

#include "QCtmTableView.h"
#include "QCtmTableItemDelegate.h"

#include <QHeaderView>
#include <QHoverEvent>
#include <QPersistentModelIndex>

QCtmTableView::QCtmTableView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setObjectName("hHeader");
    verticalHeader()->setObjectName("vHeader");

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(24);

    m_delegate = new QCtmTableItemDelegate(this);
    setItemDelegate(m_delegate);
}

QCtmTableView::~QCtmTableView()
{
}

bool QCtmTableView::viewportEvent(QEvent *event)
{
    switch (event->type()) 
    {
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
        setHoverIndex(indexAt(static_cast<QHoverEvent*>(event)->pos()));
        break;
    case QEvent::HoverLeave:
    case QEvent::Leave:
        setHoverIndex(QModelIndex());
        break;
    case QEvent::Wheel:
        setHoverIndex(QModelIndex());
        break;
    default:
        break;
    }
    return QTableView::viewportEvent(event);
}

void QCtmTableView::setHoverIndex(const QModelIndex& index)
{
    if (!model())
        return;
    if (m_hover.isValid())
    {
        if (this->selectionBehavior() == QAbstractItemView::SelectRows)
        {
            for (int i = 0;i<this->model()->columnCount();i++)
            {
                auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(model()->index(m_hover.row(), i)));
                if(itd)
                    itd->setHoverIndex(QModelIndex());
            }
        }
        else
        {
            auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(m_hover));
            if(itd)
                itd->setHoverIndex(QModelIndex());
        }
    }
    m_hover = index;
    if (m_hover.isValid())
    {
        if (this->selectionBehavior() == QAbstractItemView::SelectRows)
        {
            for (int i = 0; i < this->model()->columnCount(); i++)
            {
                auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(model()->index(m_hover.row(), i)));
                if(itd)
                    itd->setHoverIndex(m_hover);
            }
        }
        else
        {
            auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(m_hover));
            if(itd)
                itd->setHoverIndex(m_hover);
        }
    }
}
