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
#include "QCtmAbstractMultiPageTableModel.h"

#include <QDataStream>
#include <QMimeData>

/*!
    \class      QCtmAbstractMultiPageTableModel
    \brief      分页 TableModel 基类.
    \inherits   QCtmAbstractMultiPageItemModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmAbstractMultiPageTableModel.h
*/

/*!
    \reimp
*/
QModelIndex QCtmAbstractMultiPageTableModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

/*!
    \reimp
*/
QModelIndex QCtmAbstractMultiPageTableModel::sibling(int row, int column, const QModelIndex& idx) const { return index(row, column); }

/*!
    \reimp
*/
bool QCtmAbstractMultiPageTableModel::dropMimeData(const QMimeData* data,
                                                   Qt::DropAction action,
                                                   int row,
                                                   int column,
                                                   const QModelIndex& parent)
{
    if (!data || !(action == Qt::CopyAction || action == Qt::MoveAction))
        return false;

    QStringList types = mimeTypes();
    if (types.isEmpty())
        return false;
    QString format = types.at(0);
    if (!data->hasFormat(format))
        return false;

    QByteArray encoded = data->data(format);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDataStream stream(&encoded, QDataStream::ReadOnly);
#else
    QDataStream stream(&encoded, QIODevice::ReadOnly);
#endif

    // if the drop is on an item, replace the data in the items
    if (parent.isValid() && row == -1 && column == -1)
    {
        int top  = INT_MAX;
        int left = INT_MAX;
        QList<int> rows, columns;
        QList<QMap<int, QVariant>> data;

        while (!stream.atEnd())
        {
            int r, c;
            QMap<int, QVariant> v;
            stream >> r >> c >> v;
            rows.append(r);
            columns.append(c);
            data.append(v);
            top  = qMin(r, top);
            left = qMin(c, left);
        }

        for (int i = 0; i < data.size(); ++i)
        {
            int r = (rows.at(i) - top) + parent.row();
            int c = (columns.at(i) - left) + parent.column();
            if (hasIndex(r, c))
                setItemData(index(r, c), data.at(i));
        }

        return true;
    }

    // otherwise insert new rows for the data
    return decodeData(row, column, parent, stream);
}

/*!
    \reimp
*/
Qt::ItemFlags QCtmAbstractMultiPageTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags f = QCtmAbstractMultiPageItemModel::flags(index);
    if (index.isValid())
        f |= Qt::ItemNeverHasChildren;
    return f;
}

/*!
    \reimp
*/
QModelIndex QCtmAbstractMultiPageTableModel::parent(const QModelIndex& child) const { return QModelIndex(); }

/*!
    \reimp
*/
bool QCtmAbstractMultiPageTableModel::hasChildren(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return rowCount(parent) > 0 && columnCount(parent) > 0;
    return false;
}
