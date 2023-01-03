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
#include "QCtmClassifyTreeItem.h"
#include "QCtmClassifyTreeModel.h"

struct QCtmClassifyTreeItem::Impl
{
    QCtmClassifyTreeModel* model { nullptr };
    QCtmClassifyTreeItem* parent { nullptr };

    QIcon icon;
    QString text;
};

/*!
    \class      QCtmClassifyTreeItem
    \brief      归类树子项.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmClassifyTreeItem.h
*/

/*!
    \enum       QCtmClassifyTreeItem::ItemType
                项目类型.
    \value      Group
                分组项目.
    \value      Icon
                图标项目.
*/

/*!
    \fn         virtual int QCtmClassifyTreeItem::itemType() const = 0;
    \brief      返回项目类型.
*/

/*!
    \brief      构造函数.
*/
QCtmClassifyTreeItem::QCtmClassifyTreeItem() : m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      构造函数，设置显示文本 \a text.
*/
QCtmClassifyTreeItem::QCtmClassifyTreeItem(const QString& text) : QCtmClassifyTreeItem() { m_impl->text = text; }

/*!
    \brief      构造函数，设置显示文本 \a text 和图标 \a icon.
*/
QCtmClassifyTreeItem::QCtmClassifyTreeItem(const QIcon& icon, const QString& text) : QCtmClassifyTreeItem()
{
    m_impl->icon = icon;
    m_impl->text = text;
}

/*!
    \brief      析构函数.
*/
QCtmClassifyTreeItem::~QCtmClassifyTreeItem() {}

/*!
    \brief      返回数据 model.
*/
QCtmClassifyTreeModel* QCtmClassifyTreeItem::model() const
{
    if (m_impl->parent)
        return m_impl->parent->model();
    return m_impl->model;
}

/*!
    \brief      返回 index.
*/
QModelIndex QCtmClassifyTreeItem::index() const { return m_impl->model->indexFromItem(this); }

/*!
    \brief      返回父项目.
*/
QCtmClassifyTreeItem* QCtmClassifyTreeItem::parent() const { return m_impl->parent; }

/*!
    \brief      设置 \a model.
*/
void QCtmClassifyTreeItem::setModel(QCtmClassifyTreeModel* model) { m_impl->model = model; }

/*!
    \brief      设置父项目 \a parent.
*/
void QCtmClassifyTreeItem::setParent(QCtmClassifyTreeItem* parent) { m_impl->parent = parent; }

/*!
    \brief      设置显示文本 \a text.
    \sa         text
*/
void QCtmClassifyTreeItem::setText(const QString& text)
{
    m_impl->text = text;
    if (model())
    {
        auto index = this->index();
        model()->dataChanged(index, index);
    }
}

/*!
    \brief      返回显示文本.
    \sa         setText
*/
const QString& QCtmClassifyTreeItem::text() const { return m_impl->text; }

/*!
    \brief      设置图标 \a icon.
    \sa         icon
*/
void QCtmClassifyTreeItem::setIcon(const QIcon& icon)
{
    m_impl->icon = icon;
    if (model())
    {
        auto index = this->index();
        model()->dataChanged(index, index);
    }
}

/*!
    \brief      返回显示图标.
    \sa         setIcon
*/
const QIcon& QCtmClassifyTreeItem::icon() const { return m_impl->icon; }

int QCtmClassifyTreeIconItem::itemType() const { return ItemType::Icon; }

struct QCtmClassifyTreeGroupItem::Impl
{
    std::vector<QCtmClassifyTreeItem*> items;
};

QCtmClassifyTreeGroupItem::QCtmClassifyTreeGroupItem() : m_impl(std::make_unique<Impl>()) {}

QCtmClassifyTreeGroupItem::QCtmClassifyTreeGroupItem(const QString& text) : QCtmClassifyTreeIconItem(text), m_impl(std::make_unique<Impl>())
{
}

QCtmClassifyTreeGroupItem::QCtmClassifyTreeGroupItem(const QIcon& icon, const QString& text)
    : QCtmClassifyTreeIconItem(icon, text), m_impl(std::make_unique<Impl>())
{
}

QCtmClassifyTreeGroupItem::~QCtmClassifyTreeGroupItem()
{
    for (auto item : m_impl->items)
    {
        delete item;
    }
}

int QCtmClassifyTreeGroupItem::itemType() const { return ItemType::Group; }

void QCtmClassifyTreeGroupItem::addChild(QCtmClassifyTreeItem* item) { insertChild(count(), item); }

void QCtmClassifyTreeGroupItem::insertChild(int row, QCtmClassifyTreeItem* item)
{
    m_impl->items.insert(m_impl->items.begin() + row, item);
    item->setParent(this);
    if (model())
        model()->reset();
}

void QCtmClassifyTreeGroupItem::removeChild(QCtmClassifyTreeItem* item)
{
    if (auto it = std::find(m_impl->items.begin(), m_impl->items.end(), item); it != m_impl->items.end())
    {
        delete *it;
        m_impl->items.erase(it);
        if (model())
            model()->reset();
    }
}

QCtmClassifyTreeItem* QCtmClassifyTreeGroupItem::child(int row) const
{
    if (row >= m_impl->items.size())
        return nullptr;
    return m_impl->items[row];
}

int QCtmClassifyTreeGroupItem::rowOf(const QCtmClassifyTreeItem* item) const
{
    auto it = std::find(m_impl->items.begin(), m_impl->items.end(), item);
    return it == m_impl->items.end() ? -1 : std::distance(m_impl->items.begin(), it);
}

int QCtmClassifyTreeGroupItem::count() const { return static_cast<int>(m_impl->items.size()); }

void QCtmClassifyTreeGroupItem::clear()
{
    m_impl->items.clear();
    if (model())
        model()->reset();
}
