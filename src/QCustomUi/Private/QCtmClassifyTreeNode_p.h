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
    QCtmClassifyTreeGroup* m_parent{ nullptr };
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
    bool m_expand{ false };
};

class QCtmClassifyTreeIcon : public QCtmClassifyTreeNode
{
public:
    using QCtmClassifyTreeNode::QCtmClassifyTreeNode;

    QCtmClassifyTreeItem::ItemType nodeType() const override { return QCtmClassifyTreeItem::ItemType::Icon; }
};
