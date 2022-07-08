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

#include "qcustomui_global.h"

#include <QPen>
#include <QWidget>

class QCtmAbstractSeries;
class QCtmChartLayer;
class QCUSTOMUI_EXPORT QCtmAbstractChartView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont)
    Q_PROPERTY(QPen titlePen READ titlePen WRITE setTitlePen)
public:
    QCtmAbstractChartView(QWidget* parent);
    ~QCtmAbstractChartView();

    void setTitle(const QString& title);
    const QString& title() const;
    void setTitleFont(const QFont& font);
    const QFont& titleFont() const;
    void setTitlePen(const QPen& pen);
    const QPen& titlePen() const;
    virtual void addSeries(QCtmAbstractSeries* series);
    const QList<QCtmAbstractSeries*>& series() const;
    void removeSeries(QCtmAbstractSeries* series);
    void removeSeries(int i);
    void addLayer(QCtmChartLayer* layer);
    void insertLayer(int index, QCtmChartLayer* layer);
    void removeLayer(QCtmChartLayer* layer);
    const QList<QCtmChartLayer*>& layers() const;
public slots:
    void replot();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
