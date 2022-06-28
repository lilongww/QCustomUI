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

#include "QCtmChartItem.h"

class QCtmChartLayout;
class QCUSTOMUI_EXPORT QCtmChartLayer : public QCtmChartItem
{
    Q_OBJECT
public:
    QCtmChartLayer(QCtmAbstractChartView* parent);
    ~QCtmChartLayer();

    void setLayout(QCtmChartLayout* layout);
    QCtmChartLayout* layout() const;
    void replot();

protected:
    void onResized(const QSize& size) override;
    void draw(QPainter* p) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend QCtmAbstractChartView;
};