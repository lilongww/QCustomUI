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

#include <QAbstractTableModel>

#include <memory>

using QCtmAbstractMessageTipDataPtr = std::shared_ptr<class QCtmAbstractMessageTipData>;

class QCUSTOMUI_EXPORT QCtmAbstractMessageTipModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool reversedOrder READ reversedOrder WRITE setReversedOrder)
public:
    QCtmAbstractMessageTipModel(QObject* parent);
    ~QCtmAbstractMessageTipModel();

    void addMessage(QCtmAbstractMessageTipDataPtr msg);
    void insertMessage(int index, QCtmAbstractMessageTipDataPtr msg);
    void removeMessage(QCtmAbstractMessageTipDataPtr msg);
    QCtmAbstractMessageTipDataPtr message(int row) const;
    void clear();
    void setMaximumCount(int count);
    int maximumCount() const;
    void setReversedOrder(bool re);
    bool reversedOrder() const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData([[maybe_unused]] const QModelIndex& index,
                 [[maybe_unused]] const QVariant& value,
                 [[maybe_unused]] int role = Qt::EditRole) override
    {
        return false;
    }
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
