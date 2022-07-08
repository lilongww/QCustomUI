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
#include "QCtmAbstractChartView.h"
#include "QCtmAbstractSeries.h"
#include "QCtmChartLayer.h"

#include <QFont>
#include <QImage>
#include <QPainter>

struct QCtmAbstractChartView::Impl
{
    QString title;
    QFont titleFont;
    QPen titlePen;
    QList<QCtmAbstractSeries*> series;
    QList<QCtmChartLayer*> layers;
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

void QCtmAbstractChartView::addLayer(QCtmChartLayer* layer)
{
    if (!layer->parent())
        layer->setParent(this);
    m_impl->layers.append(layer);
}

void QCtmAbstractChartView::insertLayer(int index, QCtmChartLayer* layer) { m_impl->layers.insert(index, layer); }

void QCtmAbstractChartView::removeLayer(QCtmChartLayer* layer) { m_impl->layers.removeOne(layer); }

const QList<QCtmChartLayer*>& QCtmAbstractChartView::layers() const { return m_impl->layers; }

void QCtmAbstractChartView::replot()
{
    for (auto layer : m_impl->layers)
    {
        if (layer->isVisible())
            layer->replot();
    }
    update();
}

void QCtmAbstractChartView::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    for (auto layer : m_impl->layers)
    {
        if (layer->isVisible())
            layer->draw(&p);
    }
}

void QCtmAbstractChartView::resizeEvent(QResizeEvent* event)
{
    for (auto layer : m_impl->layers)
    {
        layer->setGeometry(0, 0, this->width(), this->height());
    }
    replot();
}
