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
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStyleOption>

struct QCtmFlowChartBackgroundItem::Impl
{
    QBrush background { Qt::white };
    QPen borderPen;
    QGraphicsDropShadowEffect* effect;
    QPen tickPen { 0xe5e5e5 };
    QPen subTickPen { 0xfcfcfc };
    int tickCount { 10 };
    int subTickCount { 4 };
    inline Impl(QGraphicsObject* p) : effect(new QGraphicsDropShadowEffect(p))
    {
        effect->setOffset(0, 0);
        effect->setBlurRadius(10);
        p->setGraphicsEffect(effect);
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

void QCtmFlowChartBackgroundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /*= nullptr*/)
{
    drawBackground(painter, option, widget);
    drawTicks(painter, option, widget);
}

QRectF QCtmFlowChartBackgroundItem::boundingRect() const { return QRectF { 0.0, 0.0, 1485, 1050 }; }

void QCtmFlowChartBackgroundItem::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->fillRect(option->rect, m_impl->background);
}

void QCtmFlowChartBackgroundItem::drawTicks(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // QPainterPath path;
    // qreal rowStep = rect.height() / m_impl->rowCount;
    // qreal colStep = rect.width() / m_impl->columnCount;
    // for (int row = 0; row < m_impl->rowCount; ++row)
    //{
    //     auto y = rect.top() + rowStep * row;
    //     path.moveTo(rect.left(), y);
    //     path.lineTo(rect.right(), y);
    // }
    // for (int col = 0; col < m_impl->columnCount; ++col)
    //{
    //     auto x = rect.left() + colStep * col;
    //     path.moveTo(x, rect.top());
    //     path.lineTo(x, rect.bottom());
    // }

    // painter->save();
    // painter->setPen(m_impl->tickPen);
    // painter->drawPath(path);
    // painter->restore();
}
