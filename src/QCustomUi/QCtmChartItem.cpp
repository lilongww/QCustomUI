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
#include "QCtmChartItem.h"
#include "QCtmAbstractChartView.h"

struct QCtmChartItem::Impl
{
    QRect geometry { 0, 0, 0, 0 };
    QSize minimumSize { 0, 0 };
    QSize maximumSize { QWIDGETSIZE_MAX, QWIDGETSIZE_MAX };
    bool visible { true };
};

QCtmChartItem::QCtmChartItem(QCtmAbstractChartView* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {}

QCtmChartItem::~QCtmChartItem() {}

void QCtmChartItem::setGeometry(const QRect& rect)
{
    m_impl->geometry = rect;
    onResized(rect.size());
}

const QRect& QCtmChartItem::geometry() const { return m_impl->geometry; }

void QCtmChartItem::setMinimumSize(const QSize& size) { m_impl->minimumSize = size; }

const QSize& QCtmChartItem::minimumSize() const { return m_impl->minimumSize; }

void QCtmChartItem::setMaximumSize(const QSize& size) { m_impl->maximumSize = size; }

const QSize& QCtmChartItem::maximumSize() const { return m_impl->maximumSize; }

void QCtmChartItem::setVisible(bool visible) { m_impl->visible = visible; }

bool QCtmChartItem::isVisible() const { return m_impl->visible; }

QCtmAbstractChartView* QCtmChartItem::chart() const { return qobject_cast<QCtmAbstractChartView*>(parent()); }

QSize QCtmChartItem::minimumSizeHint() const { return {}; }

void QCtmChartItem::onResized(const QSize& size) {}

void QCtmChartItem::onMoved(const QPoint& point) {}
