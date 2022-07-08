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

#include "QCtmAbstractSeries.h"

#include <QObject>

class QCtmRadarChartView;
class QCUSTOMUI_EXPORT QCtmRadarSeries : public QCtmAbstractSeries
{
    Q_OBJECT

public:
    QCtmRadarSeries(QCtmRadarChartView* parent);
    ~QCtmRadarSeries();
    void setValues(const std::vector<int>& values);
    const std::vector<int>& values() const;
    void setBrush(const QBrush& brush);
    const QBrush& brush() const;
    void setBorderColor(const QColor& color);
    const QColor& borderColor() const;
    void setColor(const QColor& color) override;
signals:
    void valueChanged();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
