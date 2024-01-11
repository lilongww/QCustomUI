/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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
#include <QDateTime>
#include <QIcon>
#include <QString>
#include <QVector>

#include <optional>

struct QCtmRecentData
{
    QString name;
    QString path;
    QDateTime time;
    QIcon icon;
    bool isTop { false };
};

class QCUSTOMUI_EXPORT QCtmRecentModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Icon = Qt::DecorationRole,
        Name = Qt::UserRole + 1,
        Path,
        Time,
        isTop
    };
    enum Classification
    {
        Top,
        Today,
        Yesterday,
        Pastweek,
        Pastmonth,
        Earlier,
        ClassificationSize
    };
    explicit QCtmRecentModel(QObject* parent);
    ~QCtmRecentModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void setRecentDatas(const QVector<QCtmRecentData>& datas);
    void setRecentDatas(QVector<QCtmRecentData>&& datas);
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;

    const QVector<QCtmRecentData>& recentDatas() const;
    std::optional<QCtmRecentData> dataOfIndex(const QModelIndex& index) const;
    void search(const QString& name, Qt::CaseSensitivity cs);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};