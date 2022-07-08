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
#include "QCtmChartLayout.h"

struct QCtmChartLayout::Impl
{
    QMargins margins { 0, 0, 0, 0 };
};

QCtmChartLayout::QCtmChartLayout(QCtmAbstractChartView* parent) : QCtmChartItem(parent), m_impl(std::make_unique<Impl>()) {}

QCtmChartLayout::~QCtmChartLayout() {}

void QCtmChartLayout::invalidate() { calcSize(this->geometry().size()); }

void QCtmChartLayout::setMargins(const QMargins& margin) { m_impl->margins = margin; }

const QMargins& QCtmChartLayout::margins() const { return m_impl->margins; }

void QCtmChartLayout::onResized(const QSize& size) { calcSize(size); }
