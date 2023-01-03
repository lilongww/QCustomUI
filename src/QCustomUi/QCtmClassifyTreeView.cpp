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

#include "QCtmClassifyTreeView.h"
#include "Private/QCtmClassifyTreeNode_p.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>

Q_DECLARE_METATYPE(QCtmClassifyTreeItem::ItemType)

constexpr int groupHeight      = 30;
constexpr int branchArrawWidth = 30;
constexpr int itemIconSize     = 16;

struct QCtmClassifyTreeView::Impl
{
    int horizontalSpace { 0 };
    int verticalSpace { 0 };
    QSize iconNodeSize { 32, 32 };
    std::map<QModelIndex, QCtmClassifyTreeNodePtr> nodes;
    std::vector<QCtmClassifyTreeNodePtr> rootNodes;
    int rangeMax { 0 };
    int indentation { 20 };
    QModelIndex hoverIndex;
    bool mousePressed { false };
};

/*!
    \class      QCtmClassifyTreeView
    \brief      归类树视图.
    \inherits   QAbstractItemView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmClassifyTreeView.h

    \b          {截图:}
    \image      QCtmClassifyTreeViewDetail.png
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmClassifyTreeView::QCtmClassifyTreeView(QWidget* parent) : QAbstractItemView(parent), m_impl(std::make_unique<Impl>())
{
    this->setMouseTracking(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*!
    \brief      析构函数.
*/
QCtmClassifyTreeView::~QCtmClassifyTreeView() {}

/*!
    \brief      设置水平间隔像素 \a space.
    \sa         horizontalSpace
*/
void QCtmClassifyTreeView::setHorizontalSpace(int space) { m_impl->horizontalSpace = space; }

/*!
    \brief      返回水平间隔像素.
    \sa         setHorizontalSpace
*/
int QCtmClassifyTreeView::horizontalSpace() const { return m_impl->horizontalSpace; }

/*!
    \brief      设置垂直间隔像素 \a space.
    \sa         verticalSpace
*/
void QCtmClassifyTreeView::setVerticalSpace(int space) { m_impl->verticalSpace = space; }

/*!
    \brief      返回垂直间隔像素.
    \sa         setVerticalSpace
*/
int QCtmClassifyTreeView::verticalSpace() const { return m_impl->verticalSpace; }

/*!
    \brief      设置图标项目的大小 \a size.
    \sa         iconItemSize
*/
void QCtmClassifyTreeView::setIconItemSize(const QSize& size) { m_impl->iconNodeSize = size; }

/*!
    \brief      返回图标项目的大小.
    \sa         setIconItemSize
*/
QSize QCtmClassifyTreeView::iconItemSize() const { return m_impl->iconNodeSize; }

/*!
    \brief      设置缩进像素 \a i.
    \sa         indentation
*/
void QCtmClassifyTreeView::setIndentation(int i) { m_impl->indentation = i; }

/*!
    \brief      返回缩进像素.
    \sa         setIndentation
*/
int QCtmClassifyTreeView::indentation() const { return m_impl->indentation; }

/*!
    \reimp
*/
void QCtmClassifyTreeView::setModel(QAbstractItemModel* model)
{
    if (!model || model == this->model())
        return;
    connect(model, &QAbstractItemModel::modelReset, this, &QCtmClassifyTreeView::createNodes);
    connect(model, &QAbstractItemModel::modelReset, this, &QCtmClassifyTreeView::relayoutNodes);
    QAbstractItemView::setModel(model);
    createNodes();
}

/*!
    \reimp
*/
QModelIndex QCtmClassifyTreeView::indexAt(const QPoint& point) const
{
    QPoint pos(point.x() + horizontalOffset(), point.y() + verticalOffset());
    std::function<QCtmClassifyTreeNodePtr(const std::vector<QCtmClassifyTreeNodePtr>&)> visitor =
        [&](const std::vector<QCtmClassifyTreeNodePtr>& children) -> QCtmClassifyTreeNodePtr
    {
        for (auto node : children)
        {
            if (node->rect().contains(pos))
                return node;
            if (node->nodeType() == QCtmClassifyTreeItem::ItemType::Group)
            {
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(node);
                if (!group->children().empty() && group->expand())
                {
                    auto last = *group->children().rbegin();
                    if (last->rect().bottom() >= point.y())
                    {
                        if (auto node = visitor(group->children()); node)
                        {
                            return node;
                        }
                    }
                }
            }
        }
        return nullptr;
    };
    auto node = visitor(m_impl->rootNodes);
    return node ? node->index() : QModelIndex();
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::scrollTo([[maybe_unused]] const QModelIndex& index,
                                    [[maybe_unused]] QAbstractItemView::ScrollHint hint /*= EnsureVisible*/)
{
}

/*!
    \reimp
*/
QRect QCtmClassifyTreeView::visualRect(const QModelIndex& index) const
{
    if (auto it = m_impl->nodes.find(index); it != m_impl->nodes.end())
    {
        auto rect = it->second->rect();
        return QRect(rect.x() - horizontalOffset(), rect.y() - verticalOffset(), rect.width(), rect.height());
    }
    return {};
}

/*!
    \brief      展开所有项目.
    \sa         expand, collapseAll
*/
void QCtmClassifyTreeView::expandAll()
{
    std::function<void(const std::vector<QCtmClassifyTreeNodePtr>&)> visitor = [&](const std::vector<QCtmClassifyTreeNodePtr>& children)
    {
        for (auto node : children)
        {
            if (node->nodeType() == QCtmClassifyTreeItem::Group)
            {
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(node);
                group->setExpand(true);
                if (!group->children().empty() && group->expand())
                {
                    visitor(group->children());
                }
            }
        }
    };
    visitor(m_impl->rootNodes);
    relayoutNodes();
    viewport()->update();
}

/*!
    \brief      展开 \a index 项目.
    \sa         expandAll, collapse
*/
void QCtmClassifyTreeView::expand(const QModelIndex& index)
{
    auto it = m_impl->nodes.find(index);
    if (it == m_impl->nodes.end())
    {
        return;
    }
    auto node = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(it->second);
    if (node)
    {
        node->setExpand(true);
        relayoutNodes();
        viewport()->update();
    }
}

/*!
    \brief      收缩所有项目.
    \sa         collapse, collapseAll
*/
void QCtmClassifyTreeView::collapseAll()
{
    std::function<void(const std::vector<QCtmClassifyTreeNodePtr>&)> visitor = [&](const std::vector<QCtmClassifyTreeNodePtr>& children)
    {
        for (auto node : children)
        {
            if (node->nodeType() == QCtmClassifyTreeItem::Group)
            {
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(node);
                group->setExpand(false);
                if (!group->children().empty() && group->expand())
                {
                    visitor(group->children());
                }
            }
        }
    };
    visitor(m_impl->rootNodes);
    relayoutNodes();
    viewport()->update();
}

/*!
    \brief      收缩 \a index 项目.
    \sa         expand, collapseAll
*/
void QCtmClassifyTreeView::collapse(const QModelIndex& index)
{
    auto it = m_impl->nodes.find(index);
    if (it == m_impl->nodes.end())
    {
        return;
    }
    auto node = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(it->second);
    if (node)
    {
        node->setExpand(false);
        relayoutNodes();
        viewport()->update();
    }
}

/*!
    \reimp
*/
int QCtmClassifyTreeView::horizontalOffset() const { return 0; }

/*!
    \reimp
*/
bool QCtmClassifyTreeView::isIndexHidden([[maybe_unused]] const QModelIndex& index) const { return false; }

/*!
    \reimp
*/
QModelIndex QCtmClassifyTreeView::moveCursor([[maybe_unused]] QAbstractItemView::CursorAction cursorAction,
                                             [[maybe_unused]] Qt::KeyboardModifiers modifiers)
{
    return {};
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::setSelection([[maybe_unused]] const QRect& rect, [[maybe_unused]] QItemSelectionModel::SelectionFlags flags) {}

/*!
    \reimp
*/
int QCtmClassifyTreeView::verticalOffset() const { return verticalScrollBar()->value(); }

/*!
    \reimp
*/
QRegion QCtmClassifyTreeView::visualRegionForSelection([[maybe_unused]] const QItemSelection& selection) const { return {}; }

/*!
    \reimp
*/
void QCtmClassifyTreeView::resizeEvent(QResizeEvent* event)
{
    relayoutNodes();
    QAbstractItemView::resizeEvent(event);
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QPainter painter(viewport());
    painter.translate(0 - horizontalScrollBar()->value(), 0 - verticalScrollBar()->value());
    std::function<void(const std::vector<QCtmClassifyTreeNodePtr>&)> drawer = [&](const std::vector<QCtmClassifyTreeNodePtr>& children)
    {
        for (auto node : children)
        {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QStyleOptionViewItem opt = viewOptions();
#else
            QStyleOptionViewItem opt;
            initViewItemOption(&opt);
#endif
            initStyleOption(node.get(), opt);
            if (opt.state.testFlag(QStyle::State_Children))
            {
                auto option = opt;
                option.rect = QRect(opt.rect.left(), opt.rect.top(), branchArrawWidth, opt.rect.height());
                opt.rect    = QRect(option.rect.right(), opt.rect.top(), opt.rect.width() - option.rect.width(), opt.rect.height());
                style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, &painter, this);
                style()->drawPrimitive(QStyle::PE_IndicatorBranch, &option, &painter, this);
            }
            this->itemDelegate()->paint(&painter, opt, node->index());
            if (node->nodeType() == QCtmClassifyTreeItem::Group)
            {
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(node);
                if (!group->children().empty() && group->expand())
                {
                    drawer(group->children());
                }
            }
        }
    };
    drawer(m_impl->rootNodes);
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::mouseMoveEvent(QMouseEvent* event)
{
    m_impl->hoverIndex = indexAt(event->pos());
    QAbstractItemView::mouseMoveEvent(event);
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_impl->mousePressed = true;
        auto index           = indexAt(event->pos());
        if (index.isValid())
        {
            if (auto it = m_impl->nodes.find(index); it != m_impl->nodes.end() && it->second->nodeType() == QCtmClassifyTreeItem::Group)
            {
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(it->second);
                if (group)
                {
                    group->setExpand(!group->expand());
                    relayoutNodes();
                    viewport()->update();
                    updateGeometries();
                }
            }
        }
    }
    QAbstractItemView::mousePressEvent(event);
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_impl->mousePressed = false;
        viewport()->update();
    }
    QAbstractItemView::mouseReleaseEvent(event);
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::leaveEvent(QEvent* event)
{
    m_impl->hoverIndex = QModelIndex();
    QAbstractItemView::leaveEvent(event);
    viewport()->update();
}

/*!
    \reimp
*/
void QCtmClassifyTreeView::updateGeometries()
{
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setSingleStep(10);
    verticalScrollBar()->setRange(0, m_impl->rangeMax);
}

/*!
    \brief      更新布局.
*/
void QCtmClassifyTreeView::relayoutNodes()
{
    std::function<QCtmClassifyTreeNodePtr(const std::vector<QCtmClassifyTreeNodePtr>&, QCtmClassifyTreeNodePtr, int)> layouter =
        [&](const std::vector<QCtmClassifyTreeNodePtr>& children,
            QCtmClassifyTreeNodePtr prevNode,
            int identation) -> QCtmClassifyTreeNodePtr
    {
        for (auto node : children)
        {
            if (node->nodeType() == QCtmClassifyTreeItem::Group)
            {
                node->setRect(prevNode ? QRect(identation,
                                               prevNode->rect().y() + prevNode->rect().height() +
                                                   (prevNode->nodeType() == QCtmClassifyTreeItem::Group ? 0 : m_impl->verticalSpace),
                                               viewport()->width(),
                                               groupHeight)
                                       : QRect(identation, 0, viewport()->width(), groupHeight));
                prevNode   = node;
                auto group = std::dynamic_pointer_cast<QCtmClassifyTreeGroup>(node);
                if (!group->children().empty() && group->expand())
                {
                    auto tmpNode = layouter(group->children(), node, identation + m_impl->indentation);
                    prevNode     = tmpNode ? tmpNode : prevNode;
                }
            }
            else if (node->nodeType() == QCtmClassifyTreeItem::Icon)
            {
                auto textSize =
                    this->fontMetrics().size(Qt::TextSingleLine | Qt::TextDontClip, node->index().data(Qt::DisplayRole).toString());
                textSize.setWidth(textSize.width() + this->fontMetrics().averageCharWidth() * 2);
                auto icon = std::dynamic_pointer_cast<QCtmClassifyTreeIcon>(node);
                if (prevNode)
                {
                    if (prevNode->nodeType() == QCtmClassifyTreeItem::Icon)
                    {
                        icon->setRect({ QPoint(prevNode->rect().right() + m_impl->horizontalSpace, prevNode->rect().y()),
                                        QSize(qMax(m_impl->iconNodeSize.width(), textSize.width()),
                                              m_impl->iconNodeSize.height() + textSize.height()) });
                        if (icon->rect().right() > viewport()->width())
                        {
                            icon->setRect({ QPoint(identation, prevNode->rect().bottom() + m_impl->verticalSpace),
                                            QSize(qMax(m_impl->iconNodeSize.width(), textSize.width()),
                                                  m_impl->iconNodeSize.height() + textSize.height()) });
                        }
                    }
                    else
                    {
                        icon->setRect({ QPoint(identation, prevNode->rect().bottom() + m_impl->verticalSpace),
                                        QSize(qMax(m_impl->iconNodeSize.width(), textSize.width()),
                                              m_impl->iconNodeSize.height() + textSize.height()) });
                    }
                }
                else
                {
                    icon->setRect(
                        { QPoint(identation, 0),
                          QSize(qMax(m_impl->iconNodeSize.width(), textSize.width()), m_impl->iconNodeSize.height() + textSize.height()) });
                }
                prevNode = icon;
            }
        }
        return prevNode;
    };

    auto last = layouter(m_impl->rootNodes, nullptr, 0);
    if (last)
    {
        m_impl->rangeMax = qMax(0, last->rect().bottom() - viewport()->height());
    }
}

/*!
    \brief      初始化 \a node 对应的风格选项 \a option.
*/
void QCtmClassifyTreeView::initStyleOption(QCtmClassifyTreeNode* node, QStyleOptionViewItem& option)
{
    auto index  = node->index();
    option.rect = node->rect();

    if (index.flags().testFlag(Qt::ItemIsEnabled))
        option.state |= QStyle::State_Enabled;
    if (node->index() == m_impl->hoverIndex)
    {
        option.state |= QStyle::State_MouseOver;
        if (m_impl->mousePressed)
            option.state |= QStyle::State_Sunken;
    }

    if (node->nodeType() == QCtmClassifyTreeItem::ItemType::Group)
    {
        auto group = dynamic_cast<QCtmClassifyTreeGroup*>(node);
        if (group)
        {
            if (!group->children().empty())
                option.state |= QStyle::State_Children;
            if (group->expand())
                option.state |= QStyle::State_Open;
        }
    }
    else if (node->nodeType() == QCtmClassifyTreeItem::ItemType::Icon)
    {
        option.showDecorationSelected = false;
        option.decorationPosition     = QStyleOptionViewItem::Top;
        option.displayAlignment       = Qt::AlignCenter;
    }
}

void QCtmClassifyTreeView::createNodes()
{
    m_impl->nodes.clear();
    m_impl->rootNodes.clear();
    if (!model())
        return;
    if (!model()->rowCount())
        return;

    std::function<QCtmClassifyTreeNodePtr(const QModelIndex&, QCtmClassifyTreeGroup*)> creator =
        [&](const QModelIndex& index, QCtmClassifyTreeGroup* parent) -> QCtmClassifyTreeNodePtr
    {
        const auto& data = index.data(Role::NodeTypeRole);
        if (!data.isValid())
        {
            qWarning() << "Node type is unknown";
            return nullptr;
        }
        auto type = data.value<QCtmClassifyTreeItem::ItemType>();
        QCtmClassifyTreeNodePtr node;
        switch (type)
        {
        case QCtmClassifyTreeItem::Group:
            {
                auto group = std::make_shared<QCtmClassifyTreeGroup>(index, parent);
                if (model()->rowCount(index))
                {
                    for (int row = 0; row < model()->rowCount(index); row++)
                    {
                        creator(model()->index(row, 0, index), group.get());
                    }
                }
                node = group;
                break;
            }
        case QCtmClassifyTreeItem::Icon:
            node = std::make_shared<QCtmClassifyTreeIcon>(index, parent);
            break;
        default:
            qWarning() << "Wrong node type";
            return nullptr;
        }
        if (parent)
        {
            parent->addChild(node);
        }
        m_impl->nodes[index] = node;
        return node;
    };

    for (int row = 0; row < model()->rowCount(); row++)
    {
        m_impl->rootNodes.push_back(creator(model()->index(row, 0), nullptr));
    }
}
