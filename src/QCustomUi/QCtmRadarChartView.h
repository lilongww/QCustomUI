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
#pragma once

#include "QCtmAbstractChartView.h"

#include <QPen>
#include <QWidget>

class QPainter;
class QCtmRadarSeries;
class QCUSTOMUI_EXPORT QCtmRadarChartView : public QCtmAbstractChartView
{
    Q_OBJECT
    Q_PROPERTY(GridType gridType READ gridType WRITE setGridType)
    Q_PROPERTY(int stepCount READ stepCount WRITE setStepCount)
    Q_PROPERTY(QMargins gridMargins READ gridMargins WRITE setGridMargins)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
    Q_PROPERTY(QPen gridPen READ gridPen WRITE setGridPen)
    Q_PROPERTY(bool alternatingColors READ alternatingColors WRITE setAlternatingColors)
    Q_PROPERTY(QBrush alternateBrush READ alternateBrush WRITE setAlternateBrush)
public:
    enum class GridType
    {
        Circle,
        Polygon
    };
    using Range     = std::pair<int, int>;
    using Indicator = std::pair<QString, Range>;

    QCtmRadarChartView(QWidget* parent);
    QCtmRadarChartView(QWidget* parent, const std::vector<Indicator>& indicators);
    ~QCtmRadarChartView();

    void setIndicator(const std::vector<Indicator>& indictors);
    const std::vector<Indicator> indictors() const;
    void setStepCount(int count);
    int stepCount() const;
    void setGridType(GridType type);
    GridType gridType() const;
    void setGridMargins(const QMargins& margins);
    const QMargins& gridMargins() const;
    QCtmRadarSeries* addSeries();
    void setLabelColor(const QColor& color);
    const QColor& labelColor() const;
    void setBackgroundBrush(const QBrush& brush);
    const QBrush backgroundBrush() const;
    void setGridPen(const QPen& gridPen);
    const QPen& gridPen() const;
    void setAlternatingColors(bool enable);
    bool alternatingColors() const;
    void setAlternateBrush(const QBrush& brush);
    const QBrush& alternateBrush() const;
    virtual QString label(int i) const;
    using QCtmAbstractChartView::addSeries;

protected:
    void paintEvent(QPaintEvent* e) override;
    virtual void drawTitle(QPainter* p, const QRectF& rect);
    virtual void drawGrid(QPainter* p, const QPointF& center, double step);
    virtual void drawValue(QPainter* p, const QBrush& brush, const QColor& borderColor, const QPolygonF& points);
    virtual void drawLabels(QPainter* p, const QPolygonF& points);
    virtual void drawLabel(QPainter* p, const QString& text, int index, int total, const QPointF& pos);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private:
    QRectF titleRect() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
