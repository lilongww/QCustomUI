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
#include "QCtmClassifyTreeModel.h"
#include "QCtmClassifyTreeItem.h"
#include "QCtmClassifyTreeView.h"

struct QCtmClassifyTreeModel::Impl
{
    std::vector<QCtmClassifyTreeItem*> items;
};

/*!
    \class      QCtmClassifyTreeModel
    \brief      标准归类树 model.
    \inherits   QAbstractItemModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmClassifyTreeModel.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmClassifyTreeModel::QCtmClassifyTreeModel(QObject* parent) : QAbstractItemModel(parent), m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
QCtmClassifyTreeModel::~QCtmClassifyTreeModel()
{
    for (auto item : m_impl->items)
        delete item;
}

/*!
    \brief      添加根节点 \a item.
    \sa         insertItem, removeItem
*/
void QCtmClassifyTreeModel::addItem(QCtmClassifyTreeItem* item) { insertItem(static_cast<int>(m_impl->items.size()), item); }

/*!
    \brief      插入根节点 \a index, \a item.
    \sa         addItem, removeItem
*/
void QCtmClassifyTreeModel::insertItem(int index, QCtmClassifyTreeItem* item)
{
    m_impl->items.insert(m_impl->items.begin() + index, item);
    item->setModel(this);
    reset();
}

/*!
    \brief      移除根节点 \a item.
    \sa         addItem, insertItem
*/
void QCtmClassifyTreeModel::removeItem(QCtmClassifyTreeItem* item)
{
    if (auto it = std::find(m_impl->items.begin(), m_impl->items.end(), item); it != m_impl->items.end())
    {
        delete *it;
        m_impl->items.erase(it);
        reset();
    }
}

/*!
    \brief      重置 model.
*/
void QCtmClassifyTreeModel::reset()
{
    beginResetModel();
    endResetModel();
}

/*!
    \brief      返回 \a item 的 index.
*/
QModelIndex QCtmClassifyTreeModel::indexFromItem(const QCtmClassifyTreeItem* item) const
{
    auto parent = item->parent();
    if (!parent)
    {
        auto it = std::find(m_impl->items.begin(), m_impl->items.end(), item);
        if (it == m_impl->items.end())
            return {};
        return createIndex(std::distance(m_impl->items.begin(), it), 0, const_cast<QCtmClassifyTreeItem*>(item));
    }
    else
    {
        auto group = dynamic_cast<QCtmClassifyTreeGroupItem*>(parent);
        if (!group)
            return {};
        return createIndex(group->rowOf(item), 0, const_cast<QCtmClassifyTreeItem*>(item));
    }
}

/*!
    \reimp
*/
int QCtmClassifyTreeModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!parent.isValid())
        return static_cast<int>(m_impl->items.size());
    auto item = reinterpret_cast<QCtmClassifyTreeItem*>(parent.internalPointer());
    if (!item)
        return 0;
    if (item->itemType() == QCtmClassifyTreeItem::Group)
    {
        auto group = dynamic_cast<QCtmClassifyTreeGroupItem*>(item);
        if (group)
            return group->count();
    }
    return 0;
}

/*!
    \reimp
*/
int QCtmClassifyTreeModel::columnCount([[maybe_unused]] const QModelIndex& parnet /*= QModelIndex()*/) const { return 1; }

/*!
    \reimp
*/
QVariant QCtmClassifyTreeModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    if (!index.isValid())
        return {};
    auto item = reinterpret_cast<QCtmClassifyTreeItem*>(index.internalPointer());
    if (!item)
        return {};
    if (role == Qt::DisplayRole)
    {
        return item->text();
    }
    else if (role == Qt::DecorationRole)
    {
        return item->icon();
    }
    else if (role == QCtmClassifyTreeView::Role::NodeTypeRole)
    {
        return item->itemType();
    }
    return {};
}

/*!
    \reimp
*/
QModelIndex QCtmClassifyTreeModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (parent.isValid())
    {
        auto group = reinterpret_cast<QCtmClassifyTreeGroupItem*>(parent.internalPointer());
        if (!group)
            return {};
        auto item = group->child(row);
        if (!item)
            return {};
        return createIndex(row, column, item);
    }
    else
    {
        if (row >= m_impl->items.size())
            return {};
        auto item = m_impl->items[row];
        return createIndex(row, column, item);
    }
}

/*!
    \reimp
*/
QModelIndex QCtmClassifyTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return {};
    auto item = reinterpret_cast<QCtmClassifyTreeItem*>(child.internalPointer());
    if (!item)
        return {};
    auto parent = item->parent();
    if (!parent)
        return {};
    return indexFromItem(parent);
}
