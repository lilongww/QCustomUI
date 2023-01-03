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

#include "qcustomui_global.h"

#include <QAbstractItemModel>
#include <QIcon>

class QCtmClassifyTreeModel;

class QCUSTOMUI_EXPORT QCtmClassifyTreeItem
{
public:
    enum ItemType
    {
        Group,
        Icon
    };
    QCtmClassifyTreeItem();
    explicit QCtmClassifyTreeItem(const QString& text);
    explicit QCtmClassifyTreeItem(const QIcon& icon, const QString& text);
    virtual ~QCtmClassifyTreeItem();
    virtual int itemType() const = 0;
    QCtmClassifyTreeModel* model() const;
    QModelIndex index() const;
    QCtmClassifyTreeItem* parent() const;
    void setText(const QString& text);
    const QString& text() const;
    void setIcon(const QIcon& icon);
    const QIcon& icon() const;

protected:
    void setModel(QCtmClassifyTreeModel* model);
    void setParent(QCtmClassifyTreeItem* parent);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class QCtmClassifyTreeGroupItem;
    friend QCtmClassifyTreeModel;
};

class QCUSTOMUI_EXPORT QCtmClassifyTreeIconItem : public QCtmClassifyTreeItem
{
public:
    using QCtmClassifyTreeItem::QCtmClassifyTreeItem;
    int itemType() const override;
};

class QCUSTOMUI_EXPORT QCtmClassifyTreeGroupItem : public QCtmClassifyTreeIconItem
{
public:
    QCtmClassifyTreeGroupItem();
    explicit QCtmClassifyTreeGroupItem(const QString& text);
    explicit QCtmClassifyTreeGroupItem(const QIcon& icon, const QString& text);
    ~QCtmClassifyTreeGroupItem();
    int itemType() const override;
    void addChild(QCtmClassifyTreeItem* item);
    void insertChild(int row, QCtmClassifyTreeItem* item);
    void removeChild(QCtmClassifyTreeItem* item);
    QCtmClassifyTreeItem* child(int row) const;
    int rowOf(const QCtmClassifyTreeItem* item) const;
    int count() const;
    void clear();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
