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

#include "QCtmAbstractMultiPageTableModel.h"

class QCUSTOMUI_EXPORT QCtmMultiPageStringListModel : public QCtmAbstractMultiPageTableModel
{
    Q_OBJECT
public:
    explicit QCtmMultiPageStringListModel(QObject* parent = nullptr);
    explicit QCtmMultiPageStringListModel(const QStringList& strings, QObject* parent = nullptr);
    explicit QCtmMultiPageStringListModel(QStringList&& strings, QObject* parent = nullptr);
    ~QCtmMultiPageStringListModel();
    void setStringList(const QStringList& strings);
    void setStringList(QStringList&& strings);
    const QStringList& stringList() const;
    int rowCount(const QModelIndex& parent /* = QModelIndex() */) const override;
    int columnCount(const QModelIndex& parent /* = QModelIndex() */) const override;
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role /* = Qt::EditRole */) override;
    int pageCount() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};