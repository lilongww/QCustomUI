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

#include <memory>

class QCtmClassifyTreeItem;

class QCUSTOMUI_EXPORT QCtmClassifyTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QCtmClassifyTreeModel(QObject* parent);
    ~QCtmClassifyTreeModel();

    void addItem(QCtmClassifyTreeItem* item);
    void insertItem(int index, QCtmClassifyTreeItem* item);
    void removeItem(QCtmClassifyTreeItem* item);
    void reset();
    QModelIndex indexFromItem(const QCtmClassifyTreeItem* item) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parnet = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
