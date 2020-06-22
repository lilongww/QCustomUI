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

#include "QCtmTableItemDelegate.h"

#include <QDebug>

/*!
    \class      QCtmTableItemDelegate
    \brief      QCtmTableItemDelegate provide a delegate for QCtmTableView to show hover row.
    \inherits   QStyledItemDelegate
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \sa         QCtmTableView
*/

/*!
    \brief      Constructs a delegate with \a parent.
*/
QCtmTableItemDelegate::QCtmTableItemDelegate(QTableView* parent)
    : QStyledItemDelegate(parent)
    , m_parent(parent)
{
}

/*!
    \brief      Destroys the delegate.
*/
QCtmTableItemDelegate::~QCtmTableItemDelegate()
{
}

/*!
    \reimp
*/
void QCtmTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    opt.state &= int(~QStyle::State_HasFocus);
    if (m_parent->selectionBehavior() == QAbstractItemView::SelectRows && index.row() == m_index.row())
    {
        opt.state |= QStyle::State_MouseOver;
    }
    else
    {
        opt.state &= int(~QStyle::State_MouseOver);
    }
    if (opt.features.testFlag(QStyleOptionViewItem::WrapText))
    {
        opt.features &= ~QStyleOptionViewItem::WrapText;
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
