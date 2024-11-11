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
#include "QCtmFlowChartBackgroundItem.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStyleOption>

struct QCtmFlowChartBackgroundItem::Impl
{
    QBrush background { Qt::white };
    QPen tickPen { 0xe5e5e5 };
    QPen subTickPen { 0xf6f6f6 };
    QPen borderPen { 0xc0c0c0 };
    QMargins margin { 20, 20, 20, 20 };
    int gridSize { 120 };
    int subTickCount { 4 };
    inline Impl(QGraphicsObject* p)
    {
        tickPen.setWidth(0);
        subTickPen.setWidth(0);
        borderPen.setWidth(0);
    }
};

QCtmFlowChartBackgroundItem::QCtmFlowChartBackgroundItem(QGraphicsItem* parent /*= nullptr*/)
    : QGraphicsObject(parent), m_impl(std::make_unique<Impl>(this))
{
}

QCtmFlowChartBackgroundItem::~QCtmFlowChartBackgroundItem() {}

void QCtmFlowChartBackgroundItem::setBackground(const QBrush& brush)
{
    m_impl->background = brush;
    update();
}

const QBrush& QCtmFlowChartBackgroundItem::background() const { return m_impl->background; }

void QCtmFlowChartBackgroundItem::setTickPen(const QPen& pen)
{
    m_impl->tickPen = pen;
    update();
}

const QPen& QCtmFlowChartBackgroundItem::tickPen() const { return m_impl->tickPen; }

void QCtmFlowChartBackgroundItem::setSubTickPen(const QPen& pen)
{
    m_impl->subTickPen = pen;
    update();
}

const QPen& QCtmFlowChartBackgroundItem::subTickPen() const { return m_impl->subTickPen; }

void QCtmFlowChartBackgroundItem::setBorderPen(const QPen& pen)
{
    m_impl->borderPen = pen;
    update();
}

const QPen& QCtmFlowChartBackgroundItem::borderPen() const { return m_impl->borderPen; }

void QCtmFlowChartBackgroundItem::setGridSize(int size)
{
    m_impl->gridSize = size;
    update();
}

int QCtmFlowChartBackgroundItem::gridSize() const { return m_impl->gridSize; }

void QCtmFlowChartBackgroundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /*= nullptr*/)
{
    drawBackground(painter, option, widget);
    auto rect = option->rect.marginsRemoved(m_impl->margin);
    drawTicks(painter, option, rect, widget);
    drawBorder(painter, option, rect, widget);
}

QRectF QCtmFlowChartBackgroundItem::boundingRect() const { return QRectF { 0.0, 0.0, 1240, 1754 }; }

void QCtmFlowChartBackgroundItem::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->fillRect(option->rect, m_impl->background);
}

void QCtmFlowChartBackgroundItem::drawBorder(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& rect, QWidget* widget)
{
    painter->save();
    painter->setPen(m_impl->borderPen);
    painter->drawRect(rect);
    painter->restore();
}

void QCtmFlowChartBackgroundItem::drawTicks(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& rect, QWidget* widget)
{
    QPainterPath tickPath, subTickPath;
    int rowCount     = rect.height() / m_impl->gridSize + 1;
    int columnCount  = rect.width() / m_impl->gridSize + 1;
    auto subtickStep = m_impl->gridSize / static_cast<double>(m_impl->subTickCount);
    for (int row = 0; row < rowCount; ++row)
    {
        auto y = rect.top() + m_impl->gridSize * row;
        if (row != 0)
        {
            tickPath.moveTo(rect.left(), y);
            tickPath.lineTo(rect.right(), y);
        }
        for (int i = 1; i < m_impl->subTickCount; ++i)
        {
            auto sy = y + i * subtickStep;
            subTickPath.moveTo(rect.left(), sy);
            subTickPath.lineTo(rect.right(), sy);
        }
    }
    for (int col = 0; col < columnCount; ++col)
    {
        auto x = rect.left() + m_impl->gridSize * col;
        if (col != 0)
        {
            tickPath.moveTo(x, rect.top());
            tickPath.lineTo(x, rect.bottom());
        }
        for (int i = 1; i < m_impl->subTickCount; ++i)
        {
            auto sx = x + i * subtickStep;
            subTickPath.moveTo(sx, rect.top());
            subTickPath.lineTo(sx, rect.bottom());
        }
    }

    painter->save();
    painter->setClipRect(rect);
    painter->setPen(m_impl->subTickPen);
    painter->drawPath(subTickPath);
    painter->setPen(m_impl->tickPen);
    painter->drawPath(tickPath);
    painter->restore();
}
