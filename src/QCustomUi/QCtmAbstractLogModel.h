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
#include <QApplication>

#include <memory>

class QCtmLogEvent;

class QCUSTOMUI_EXPORT QCtmAbstractLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCtmAbstractLogModel(const QString& objectName, QObject* parent);
    ~QCtmAbstractLogModel();
    virtual void clear() = 0;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;

protected:
    bool event(QEvent* e) override;
    virtual void logEvent(QCtmLogEvent* e);
    virtual void retranslateUi();
};
