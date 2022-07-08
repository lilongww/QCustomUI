#include "QCtmRadarChartView.h"
#include "QCtmChartHBoxLayout.h"
#include "QCtmColor.h"
#include "QCtmRadarSeries.h"

#include <QPainter>
#include <QPainterPath>
#include <qmath.h>

struct QCtmRadarChartView::Impl
{
    std::vector<Indicator> indicators {
        { "Test1", { 0, 100 } },
        { "Test2", { 0, 100 } },
        { "Test3", { 0, 100 } },
    };
    GridType gridType { GridType::Circle };
    int stepCount { 5 };
    QBrush backgroundBrush { Qt::white };
    QPen gridPen { 0xCDCDCD };
    QBrush alternateBrush { 0xEFEFEF };
    bool alternatingColors { true };
    QMargins gridMargins { 0, 0, 0, 0 };
    QColor labelColor { QColor(0, 170, 255, 255) };

    inline double angle(int i) const { return -(2 * M_PI / indicators.size() * i + 0.5 * 2 * M_PI); }
    inline QPointF pointOf(int i, const QPointF& center, double radius) const
    {
        auto a = angle(i);
        return QPointF(qSin(a) * radius + center.x(), qCos(a) * radius + center.y());
    }
    inline QPolygonF polygon(const QPointF& center, qreal radius) const
    {
        QPolygonF pg;
        for (int i = 0; i < indicators.size(); i++)
        {
            pg.push_back(pointOf(i, center, radius));
        }
        return pg;
    }
    inline QPolygonF fromValue(const QPointF& center, qreal radius, const std::vector<int>& values) const
    {
        QPolygonF pg;
        for (int i = 0; i < std::min(values.size(), indicators.size()); i++)
        {
            pg.push_back(pointOf(i, center, radius * persent(i, values)));
        }
        return pg;
    }
    inline double persent(int i, const std::vector<int>& values) const
    {
        const auto& range = indicators[i].second;
        return (static_cast<double>(values[i]) - range.first) / (static_cast<double>(range.second) - range.first);
    }
};

QCtmRadarChartView::QCtmRadarChartView(QWidget* parent) : QCtmAbstractChartView(parent), m_impl(std::make_unique<Impl>()) {}

QCtmRadarChartView::QCtmRadarChartView(QWidget* parent, const std::vector<Indicator>& indicator) : QCtmRadarChartView(parent)
{
    setIndicator(indicator);
}

QCtmRadarChartView::~QCtmRadarChartView() {}

void QCtmRadarChartView::setIndicator(const std::vector<Indicator>& indictors)
{
    m_impl->indicators = indictors;
    update();
}

const std::vector<QCtmRadarChartView::Indicator> QCtmRadarChartView::indictors() const { return m_impl->indicators; }

void QCtmRadarChartView::setStepCount(int count)
{
    m_impl->stepCount = count;
    update();
}

int QCtmRadarChartView::stepCount() const { return m_impl->stepCount; }

void QCtmRadarChartView::setGridType(GridType type)
{
    m_impl->gridType = type;
    update();
}

QCtmRadarChartView::GridType QCtmRadarChartView::gridType() const { return m_impl->gridType; }

void QCtmRadarChartView::setGridMargins(const QMargins& margins)
{
    m_impl->gridMargins = margins;
    update();
}

const QMargins& QCtmRadarChartView::gridMargins() const { return m_impl->gridMargins; }

QCtmRadarSeries* QCtmRadarChartView::addSeries()
{
    auto series = new QCtmRadarSeries(this);
    auto color  = QCtmColor::generateBackgroundColor(this->series().size());
    series->setBorderColor(color);
    color.setAlpha(50);
    series->setBrush(color);
    addSeries(series);
    return series;
}

void QCtmRadarChartView::addSeries(QCtmAbstractSeries* series) { QCtmAbstractChartView::addSeries(series); }

void QCtmRadarChartView::setLabelColor(const QColor& color)
{
    m_impl->labelColor = color;
    update();
}

const QColor& QCtmRadarChartView::labelColor() const { return m_impl->labelColor; }

void QCtmRadarChartView::setBackgroundBrush(const QBrush& brush)
{
    m_impl->backgroundBrush = brush;
    update();
}

const QBrush QCtmRadarChartView::backgroundBrush() const { return m_impl->backgroundBrush; }

void QCtmRadarChartView::setGridPen(const QPen& gridPen)
{
    m_impl->gridPen = gridPen;
    update();
}

const QPen& QCtmRadarChartView::gridPen() const { return m_impl->gridPen; }

void QCtmRadarChartView::setAlternatingColors(bool enable)
{
    m_impl->alternatingColors = enable;
    update();
}

bool QCtmRadarChartView::alternatingColors() const { return m_impl->alternatingColors; }

void QCtmRadarChartView::setAlternateBrush(const QBrush& brush)
{
    m_impl->alternateBrush = brush;
    update();
}

const QBrush& QCtmRadarChartView::alternateBrush() const { return m_impl->alternateBrush; }

QString QCtmRadarChartView::label(int i) const { return m_impl->indicators[i].first; }

void QCtmRadarChartView::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    auto titleRect = this->titleRect();

    auto diameter = std::min(this->width(), static_cast<int>(this->height() - titleRect.height() - 10));
    QRectF rect((width() - diameter) / 2, (height() - diameter) / 2 + titleRect.height() + 10, diameter, diameter);
    rect        = rect.marginsRemoved(m_impl->gridMargins);
    auto radius = rect.width() / 2.0;
    auto step   = radius / m_impl->stepCount;

    drawTitle(&p, titleRect);
    drawGrid(&p, rect.center(), step);
    for (auto series : series())
    {
        auto radarSeries = qobject_cast<QCtmRadarSeries*>(series);
        if (!radarSeries)
            continue;
        auto polygon = m_impl->fromValue(rect.center(), radius, radarSeries->values());
        drawValue(&p, radarSeries->brush(), radarSeries->borderColor(), polygon);
    }
}

void QCtmRadarChartView::drawTitle(QPainter* p, const QRectF& rect)
{
    p->setPen(titlePen());
    p->setFont(titleFont());
    QTextOption opt;
    opt.setAlignment(Qt::AlignCenter);
    p->drawText(rect, title(), opt);
}

void QCtmRadarChartView::drawGrid(QPainter* p, const QPointF& center, double step)
{
    p->setBrush(m_impl->backgroundBrush);
    p->setPen(m_impl->gridPen);
    auto polygon = m_impl->polygon(center, step * m_impl->stepCount);

    for (int i = m_impl->stepCount; i > 0; i--)
    {
        if (m_impl->alternatingColors)
            p->setBrush(i % 2 == m_impl->stepCount % 2 ? m_impl->backgroundBrush : m_impl->alternateBrush);
        if (m_impl->gridType == GridType::Circle)
            p->drawEllipse(center, i * step, i * step);
        else
            p->drawPolygon(m_impl->polygon(center, step * i));
    }

    std::for_each(polygon.begin(), polygon.end(), [&center, p](const auto& point) { p->drawLine(center, point); });
    drawLabels(p, polygon);
}

void QCtmRadarChartView::drawValue(QPainter* p, const QBrush& brush, const QColor& borderColor, const QPolygonF& points)
{
    p->setBrush(brush);
    p->setPen(borderColor);
    p->drawPolygon(points);
    p->setBrush(borderColor);
    std::for_each(points.begin(), points.end(), [&p](const auto& center) { p->drawEllipse(center, 3, 3); });
}

void QCtmRadarChartView::drawLabels(QPainter* p, const QPolygonF& points)
{
    p->setFont(this->font());
    p->setPen(m_impl->labelColor);
    for (int i = 0; i < points.size(); i++)
    {
        drawLabel(p, label(i), i, points.size(), points[i]);
    }
}

void QCtmRadarChartView::drawLabel(QPainter* p, const QString& text, int index, int total, const QPointF& pos)
{
    QTextOption opt;
    QFontMetricsF fm(this->font());
    auto h = fm.height();
    auto w = fm.horizontalAdvance(text);
    if (!index)
    {
        opt.setAlignment(Qt::AlignCenter);
        p->drawText(QRectF(pos.x() - w / 2, pos.y() - h, w, h), text, opt);
    }
    else if (!(total % 2) && total / 2 == index)
    {
        opt.setAlignment(Qt::AlignCenter);
        p->drawText(QRectF(pos.x() - w / 2, pos.y(), w, h), text, opt);
    }
    else if (index < total / 2)
    {
        opt.setAlignment(Qt::AlignLeft | Qt::AlignCenter);
        p->drawText(QRectF(pos.x(), pos.y() - h / 2, w, h), text, opt);
    }
    else
    {
        opt.setAlignment(Qt::AlignRight | Qt::AlignCenter);
        p->drawText(QRectF(pos.x() - w, pos.y() - h / 2, w, h), text, opt);
    }
}

QSize QCtmRadarChartView::minimumSizeHint() const
{
    return QSize(100 + m_impl->gridMargins.left() + m_impl->gridMargins.right(),
                 100 + titleRect().height() + m_impl->gridMargins.top() + m_impl->gridMargins.bottom());
}

QSize QCtmRadarChartView::sizeHint() const { return minimumSizeHint(); }

QRectF QCtmRadarChartView::titleRect() const
{
    QFontMetricsF fm(titleFont());
    return QRectF(m_impl->gridMargins.left(),
                  m_impl->gridMargins.top(),
                  width() - m_impl->gridMargins.left() - m_impl->gridMargins.right(),
                  fm.height());
}
