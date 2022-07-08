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
#include "QCtmAbstractSeries.h"
#include "QCtmAbstractChartView.h"

struct QCtmAbstractSeries::Impl
{
    QString name;
    QColor color;
};

QCtmAbstractSeries::QCtmAbstractSeries(QCtmAbstractChartView* parent) : QCtmChartItem(parent), m_impl(std::make_unique<Impl>()) {}

QCtmAbstractSeries::~QCtmAbstractSeries() {}

void QCtmAbstractSeries::setName(const QString& name) { m_impl->name = name; }

const QString& QCtmAbstractSeries::name() const { return m_impl->name; }

void QCtmAbstractSeries::setColor(const QColor& color) { m_impl->color = color; }

const QColor& QCtmAbstractSeries::color() const { return m_impl->color; }
