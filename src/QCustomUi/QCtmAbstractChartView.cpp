#include "QCtmAbstractChartView.h"
#include "QCtmAbstractSeries.h"

struct QCtmAbstractChartView::Impl
{
    QString title;
    QList<QCtmAbstractSeries*> series;
};

QCtmAbstractChartView::QCtmAbstractChartView(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>()) {}

QCtmAbstractChartView::~QCtmAbstractChartView() {}

void QCtmAbstractChartView::setTitle(const QString& title) { m_impl->title = title; }

const QString& QCtmAbstractChartView::title() const { return m_impl->title; }

void QCtmAbstractChartView::addSeries(QCtmAbstractSeries* series)
{
    series->setParent(this);
    m_impl->series.push_back(series);
}

const QList<QCtmAbstractSeries*>& QCtmAbstractChartView::series() const { return m_impl->series; }

void QCtmAbstractChartView::removeSeries(QCtmAbstractSeries* series) { m_impl->series.removeOne(series); }

void QCtmAbstractChartView::removeSeries(int i) { m_impl->series.removeAt(i); }
