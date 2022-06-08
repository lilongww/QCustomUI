#include "QCtmAbstractChartView.h"
#include "QCtmAbstractSeries.h"

#include <QFont>

struct QCtmAbstractChartView::Impl
{
    QString title;
    QFont titleFont;
    QPen titlePen;
    QList<QCtmAbstractSeries*> series;
};

QCtmAbstractChartView::QCtmAbstractChartView(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->titleFont = this->font();
    m_impl->titleFont.setBold(true);
    m_impl->titleFont.setPixelSize(20);
}

QCtmAbstractChartView::~QCtmAbstractChartView() {}

void QCtmAbstractChartView::setTitle(const QString& title) { m_impl->title = title; }

const QString& QCtmAbstractChartView::title() const { return m_impl->title; }

void QCtmAbstractChartView::setTitleFont(const QFont& font) { m_impl->titleFont = font; }

const QFont& QCtmAbstractChartView::titleFont() const { return m_impl->titleFont; }

void QCtmAbstractChartView::setTitlePen(const QPen& pen) { m_impl->titlePen = pen; }

const QPen& QCtmAbstractChartView::titlePen() const { return m_impl->titlePen; }

void QCtmAbstractChartView::addSeries(QCtmAbstractSeries* series)
{
    series->setParent(this);
    m_impl->series.push_back(series);
}

const QList<QCtmAbstractSeries*>& QCtmAbstractChartView::series() const { return m_impl->series; }

void QCtmAbstractChartView::removeSeries(QCtmAbstractSeries* series) { m_impl->series.removeOne(series); }

void QCtmAbstractChartView::removeSeries(int i) { m_impl->series.removeAt(i); }
