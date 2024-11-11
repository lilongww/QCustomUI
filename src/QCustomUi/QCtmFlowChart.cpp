/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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
#include "QCtmFlowChart.h"
#include "Private/QCtmFlowChartBackgroundItem.h"

#include <QGraphicsScene>
#include <QWheelEvent>

struct QCtmFlowChart::Impl
{
    QCtmFlowChartBackgroundItem* backgroundItem;
    QGraphicsScene* scene;
    double scale { 1.0 };
};

QCtmFlowChart::QCtmFlowChart(QWidget* parent) : m_impl(std::make_unique<Impl>())
{
    this->setScene(m_impl->scene = new QGraphicsScene(this));
    m_impl->scene->addItem(m_impl->backgroundItem = new QCtmFlowChartBackgroundItem);
}

QCtmFlowChart::~QCtmFlowChart() {}

void QCtmFlowChart::wheelEvent(QWheelEvent* event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull() && event->modifiers().testFlag(Qt::ControlModifier))
    {
        QPoint numSteps = numDegrees / 15;
        auto sc         = 1.0 + numSteps.y() * 0.1;
        m_impl->scale *= sc;
        scale(sc, sc);
        return;
    }
    QGraphicsView::wheelEvent(event);
}
