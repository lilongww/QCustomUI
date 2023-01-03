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

#pragma once

#include "QCtmClassifyTreeItem.h"

#include <QModelIndex>

#include <memory>
#include <vector>

class QCtmClassifyTreeGroup;

class QCtmClassifyTreeNode
{
public:
    QCtmClassifyTreeNode(const QModelIndex& index, QCtmClassifyTreeGroup* parent = nullptr) : m_index(index), m_parent(parent) {}
    virtual ~QCtmClassifyTreeNode() {}

    virtual QCtmClassifyTreeItem::ItemType nodeType() const = 0;
    inline void setParent(QCtmClassifyTreeGroup* parent) { m_parent = parent; }
    inline QCtmClassifyTreeGroup* parent() const { return m_parent; }
    inline void setRect(const QRect& rect) { m_rect = rect; }
    inline QRect rect() const { return m_rect; }
    inline const QModelIndex& index() const { return m_index; }

protected:
    QModelIndex m_index;
    QCtmClassifyTreeGroup* m_parent { nullptr };
    QRect m_rect;
};

using QCtmClassifyTreeNodePtr = std::shared_ptr<QCtmClassifyTreeNode>;

class QCtmClassifyTreeGroup : public QCtmClassifyTreeNode
{
public:
    using QCtmClassifyTreeNode::QCtmClassifyTreeNode;

    inline void addChild(QCtmClassifyTreeNodePtr child) { m_children.push_back(child); }
    inline void removeChild(QCtmClassifyTreeNodePtr child) { std::erase(m_children, child); }
    inline const std::vector<QCtmClassifyTreeNodePtr>& children() const { return m_children; }
    inline void setExpand(bool expand) { m_expand = expand; }
    inline bool expand() const { return m_expand; }

    QCtmClassifyTreeItem::ItemType nodeType() const override { return QCtmClassifyTreeItem::ItemType::Group; }

protected:
    std::vector<QCtmClassifyTreeNodePtr> m_children;
    bool m_expand { false };
};

class QCtmClassifyTreeIcon : public QCtmClassifyTreeNode
{
public:
    using QCtmClassifyTreeNode::QCtmClassifyTreeNode;

    QCtmClassifyTreeItem::ItemType nodeType() const override { return QCtmClassifyTreeItem::ItemType::Icon; }
};
