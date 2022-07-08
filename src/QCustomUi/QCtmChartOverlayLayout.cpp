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

#include "QCtmChartOverlayLayout.h"

struct QCtmChartOverlayLayout::Impl
{
    std::vector<QCtmChartItem*> items;
};

QCtmChartOverlayLayout::QCtmChartOverlayLayout(QCtmAbstractChartView* parent) : QCtmChartLayout(parent), m_impl(std::make_unique<Impl>()) {}

QCtmChartOverlayLayout::~QCtmChartOverlayLayout() {}

void QCtmChartOverlayLayout::addItem(QCtmChartItem* item) { m_impl->items.push_back(item); }

void QCtmChartOverlayLayout::removeItem(QCtmChartItem* item) { std::erase(m_impl->items, item); }

QCtmChartItem* QCtmChartOverlayLayout::itemAt(int index) const { return m_impl->items.at(index); }

void QCtmChartOverlayLayout::calcSize(const QSize& size)
{
    for (auto item : m_impl->items)
    {
        item->setGeometry(this->geometry());
    }
}

void QCtmChartOverlayLayout::draw(QPainter* p)
{
    for (auto item : m_impl->items)
    {
        if (item->isVisible())
            item->draw(p);
    }
}
