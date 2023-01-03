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
#include "QCtmMultiPageStringListModel.h"

#include <algorithm>

struct QCtmMultiPageStringListModel::Impl
{
    QStringList strings;
    inline Impl() {}
    inline Impl(const QStringList& ss) : strings(ss) {}
    inline Impl(QStringList&& ss) : strings(std::forward<QStringList&&>(ss)) {}
};

/*!
    \class      QCtmMultiPageStringListModel
    \brief      分页 StringListModel.
    \inherits   QCtmAbstractMultiPageTableModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmMultiPageStringListModel.h
*/

/*!
    \brief      构造函数 \a strings, \a parent.
*/
QCtmMultiPageStringListModel::QCtmMultiPageStringListModel(const QStringList& strings, QObject* parent /*= nullptr*/)
    : QCtmAbstractMultiPageTableModel(parent), m_impl(std::make_unique<Impl>(strings))
{
}

/*!
    \brief      构造函数 \a strings, \a parent.
*/
QCtmMultiPageStringListModel::QCtmMultiPageStringListModel(QStringList&& strings, QObject* parent /*= nullptr*/)
    : QCtmAbstractMultiPageTableModel(parent), m_impl(std::make_unique<Impl>(std::forward<QStringList&&>(strings)))
{
}

/*!
    \brief      构造函数 \a parent.
*/
QCtmMultiPageStringListModel::QCtmMultiPageStringListModel(QObject* parent /*= nullptr*/) : m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
QCtmMultiPageStringListModel::~QCtmMultiPageStringListModel() {}

/*!
    \brief      设置字符串列表 \a strings.
    \sa         stringList
*/
void QCtmMultiPageStringListModel::setStringList(const QStringList& strings)
{
    beginResetModel();
    m_impl->strings = strings;
    endResetModel();
}

/*!
    \overload
                设置字符串列表 \a strings.
    \sa         setStringList
*/
void QCtmMultiPageStringListModel::setStringList(QStringList&& strings)
{
    beginResetModel();
    m_impl->strings = std::move(strings);
    endResetModel();
}

/*!
    \brief      返回字符串列表.
    \sa         setStringList
*/
const QStringList& QCtmMultiPageStringListModel::stringList() const { return m_impl->strings; }

/*!
    \reimp
*/
int QCtmMultiPageStringListModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
    return static_cast<int>(
        std::min<size_t>(std::max<size_t>(m_impl->strings.size() - currentPage() * pageRowCount(), 0LL), pageRowCount()));
}

/*!
    \reimp
*/
int QCtmMultiPageStringListModel::columnCount(const QModelIndex& parent /* = QModelIndex() */) const { return 1; }

/*!
    \reimp
*/
QVariant QCtmMultiPageStringListModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    auto row = offset() + index.row();
    if (row < 0 || row >= m_impl->strings.size())
        return {};
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return m_impl->strings[row];
    }
    return {};
}

/*!
    \reimp
*/
bool QCtmMultiPageStringListModel::setData(const QModelIndex& index, const QVariant& value, int role /* = Qt::EditRole */)
{
    auto row = offset() + index.row();
    if (role == Qt::EditRole)
    {
        auto string = value.toString();
        if (string == m_impl->strings[row])
            return true;
        m_impl->strings[row] = std::move(string);
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
        return true;
    }
    return false;
}

/*!
    \reimp
*/
int QCtmMultiPageStringListModel::pageCount() const
{
    return m_impl->strings.size() / pageRowCount() + static_cast<bool>(m_impl->strings.size() % pageRowCount());
}
