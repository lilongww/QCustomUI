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

#include "QCtmTableView.h"
#include "QCtmTableItemDelegate.h"

#include <QHeaderView>
#include <QHoverEvent>
#include <QPersistentModelIndex>

/*!
    \class      QCtmTableView
    \brief      自定义外观的 TableView.
    \inherits   QTableView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmTableView.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmTableView::QCtmTableView(QWidget* parent) : QTableView(parent)
{
    horizontalHeader()->setObjectName("hHeader");
    verticalHeader()->setObjectName("vHeader");

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_delegate = new QCtmTableItemDelegate(this);
    setItemDelegate(m_delegate);
}

/*!
    \brief      析构函数.
*/
QCtmTableView::~QCtmTableView() {}

/*!
    \reimp
*/
bool QCtmTableView::viewportEvent(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        setHoverIndex(indexAt(static_cast<QHoverEvent*>(event)->pos()));
#else
        setHoverIndex(indexAt(static_cast<QHoverEvent*>(event)->position().toPoint()));
#endif
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

/*!
    \brief      设置 hover 的 \a index.
*/
void QCtmTableView::setHoverIndex(const QModelIndex& index)
{
    if (!model())
        return;

    auto delegate = [this](const QModelIndex& index)
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        return qobject_cast<QCtmTableItemDelegate*>(itemDelegate(index));
#else
        return qobject_cast<QCtmTableItemDelegate*>(itemDelegateForIndex(index));
#endif
    };

    if (m_hover.isValid())
    {
        if (this->selectionBehavior() == QAbstractItemView::SelectRows)
        {
            for (int i = 0; i < this->model()->columnCount(); i++)
            {
                if (auto itd = delegate(model()->index(m_hover.row(), i)); itd)
                    itd->setHoverIndex(QModelIndex());
            }
        }
        else
        {
            if (auto itd = delegate(m_hover); itd)
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
                if (auto itd = delegate(model()->index(m_hover.row(), i)); itd)
                    itd->setHoverIndex(m_hover);
            }
        }
        else
        {
            if (auto itd = delegate(m_hover); itd)
                itd->setHoverIndex(m_hover);
        }
    }
}
