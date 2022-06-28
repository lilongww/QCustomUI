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
#include "QCtmChartLayer.h"
#include "QCtmChartLayout.h"

#include <QImage>
#include <QPainter>

struct QCtmChartLayer::Impl
{
    QCtmChartLayout* layout { nullptr };
    QImage image;
};

QCtmChartLayer::QCtmChartLayer(QCtmAbstractChartView* parent) : QCtmChartItem(parent), m_impl(std::make_unique<Impl>()) {}

QCtmChartLayer::~QCtmChartLayer() {}

void QCtmChartLayer::setLayout(QCtmChartLayout* layout) { m_impl->layout = layout; }

QCtmChartLayout* QCtmChartLayer::layout() const { return m_impl->layout; }

void QCtmChartLayer::replot()
{
    if (m_impl->layout)
    {
        QPainter p(&m_impl->image);
        m_impl->layout->draw(&p);
    }
}

void QCtmChartLayer::onResized(const QSize& size)
{
    m_impl->image = QImage(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
    m_impl->image.fill(Qt::transparent);
    replot();
}

void QCtmChartLayer::draw(QPainter* p) { p->drawImage(this->geometry(), m_impl->image); }
