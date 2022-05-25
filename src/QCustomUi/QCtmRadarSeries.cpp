#include "QCtmRadarSeries.h"
#include "QCtmRadarChartView.h"

#include <QPainter>

struct QCtmRadarSeries::Impl
{
    std::vector<int> values { std::vector<int>(6, 24) };
    QBrush brush { QColor(85, 170, 127, 50) };
    QColor borderColor { QColor(0, 170, 255, 255) };
};

QCtmRadarSeries::QCtmRadarSeries(QCtmRadarChartView* parent) : QCtmAbstractSeries(parent), m_impl(std::make_unique<Impl>()) {}

QCtmRadarSeries::~QCtmRadarSeries() {}

void QCtmRadarSeries::setValues(const std::vector<int>& values)
{
    m_impl->values = values;
    emit valueChanged();
}

const std::vector<int>& QCtmRadarSeries::values() const { return m_impl->values; }

void QCtmRadarSeries::setBrush(const QBrush& brush) { m_impl->brush = brush; }

const QBrush& QCtmRadarSeries::brush() const { return m_impl->brush; }

void QCtmRadarSeries::setBorderColor(const QColor& color) { m_impl->borderColor = color; }

const QColor& QCtmRadarSeries::borderColor() const { return m_impl->borderColor; }
