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
#include "QCtmRadarSeries.h"
#include "QCtmRadarChartView.h"

#include <QPainter>

struct QCtmRadarSeries::Impl
{
    std::vector<int> values { std::vector<int>(6, 24) };
    QBrush brush { QColor(85, 170, 127, 50) };
    QColor borderColor { QColor(0, 170, 255, 255) };
};

QCtmRadarSeries::QCtmRadarSeries(QCtmRadarChartView* parent) : QCtmAbstractSeries(parent), m_impl(std::make_unique<Impl>()) {}

QCtmRadarSeries::~QCtmRadarSeries() {}

void QCtmRadarSeries::setValues(const std::vector<int>& values)
{
    m_impl->values = values;
    emit valueChanged();
}

const std::vector<int>& QCtmRadarSeries::values() const { return m_impl->values; }

void QCtmRadarSeries::setBrush(const QBrush& brush) { m_impl->brush = brush; }

const QBrush& QCtmRadarSeries::brush() const { return m_impl->brush; }

void QCtmRadarSeries::setBorderColor(const QColor& color) { m_impl->borderColor = color; }

const QColor& QCtmRadarSeries::borderColor() const { return m_impl->borderColor; }

void QCtmRadarSeries::setColor(const QColor& color)
{
    QCtmAbstractSeries::setColor(color);
    setBorderColor(color);
    auto copy = color;
    copy.setAlpha(50);
    setBrush(copy);
}
