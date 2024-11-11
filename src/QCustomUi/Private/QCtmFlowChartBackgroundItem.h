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

#pragma once

#include <QGraphicsObject>

class QCtmFlowChartBackgroundItem : public QGraphicsObject
{
public:
    explicit QCtmFlowChartBackgroundItem(QGraphicsItem* parent = nullptr);
    ~QCtmFlowChartBackgroundItem();
    void setBackground(const QBrush& brush);
    const QBrush& background() const;
    void setTickPen(const QPen& pen);
    const QPen& tickPen() const;
    void setSubTickPen(const QPen& pen);
    const QPen& subTickPen() const;
    void setBorderPen(const QPen& pen);
    const QPen& borderPen() const;
    void setGridSize(int size);
    int gridSize() const;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;
    virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual void drawBorder(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& rect, QWidget* widget);
    virtual void drawTicks(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& rect, QWidget* widget);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};