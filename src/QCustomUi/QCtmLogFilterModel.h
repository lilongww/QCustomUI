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

#include <QSortFilterProxyModel>

#include <memory>

class QCtmLogFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QCtmLogFilterModel(QObject* parent);
    ~QCtmLogFilterModel();

    void search(const QString& keyword);
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
public slots:
    void showLog(QtMsgType type, bool show);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
