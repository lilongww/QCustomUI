/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2022 LiLong                                              **
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

#include "QCtmChartLayout.h"

class QCUSTOMUI_EXPORT QCtmChartBoxLayout : public QCtmChartLayout
{
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    QCtmChartBoxLayout(Qt::Orientation orientation, QCtmAbstractChartView* parent);
    ~QCtmChartBoxLayout();

    Qt::Orientation orientation() const;
    void addItem(QCtmChartItem* item, int stretch = 0);
    void removeItem(QCtmChartItem* item);
    QCtmChartItem* itemAt(int index) const;
    void setStretch(int index, int stretch);
    size_t count() const;
    void setSpacing(int spacing);
    int spacing() const;

protected:
    void calcSize(const QSize& size) override;
    void draw(QPainter* p) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};