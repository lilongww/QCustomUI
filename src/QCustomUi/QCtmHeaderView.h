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

#include <QHeaderView>

#include <memory>

class QCUSTOMUI_EXPORT QCtmHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    QCtmHeaderView(Qt::Orientation orientation, QWidget* parent);
    ~QCtmHeaderView();

    void setModel(QAbstractItemModel* model) override;
    void setReadOnly(int logicIndex, bool enable);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    void mousePressEvent(QMouseEvent* e) override;
    QRect doCheckBoxRect(int logicalIndex) const;
private slots:
    void onModelReset();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
