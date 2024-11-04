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

#include "qcustomui_global.h"

#include <QSlider>

class QStyleOptionSlider;
class QCtmRangeSliderPrivate;
class QStylePainter;
class QCUSTOMUI_EXPORT QCtmRangeSlider : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCtmRangeSlider)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int lower READ lower WRITE setLower)
    Q_PROPERTY(int upper READ upper WRITE setUpper)
    Q_PROPERTY(bool tracking READ hasTracking WRITE setTracking)
    Q_PROPERTY(bool chunkVisible READ chunkVisible WRITE setChunkVisible)
public:
    using TickPosition = QSlider::TickPosition;
    explicit QCtmRangeSlider(QWidget* parent = nullptr);
    explicit QCtmRangeSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QCtmRangeSlider();
    Qt::Orientation orientation() const;
    void setMinimum(int minimum);
    int minimum() const;
    void setMaximum(int maximum);
    int maximum() const;
    void setLower(int value);
    int lower() const;
    void setUpper(int value);
    int upper() const;
    void setTracking(bool on);
    bool hasTracking() const;
    void setTickPosition(TickPosition pos);
    TickPosition tickPosition() const;
    void setSliderPosition(int lower, int upper);
    int lowerSliderPosition() const;
    int upperSliderPosition() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void setChunkVisible(bool visible);
    bool chunkVisible() const;
signals:
    void valueChanged(int lower, int upper);
    void rangeChanged(int minimum, int maximum);
    void sliderMoved(int lower, int upper);
public slots:
    void setRange(int minimum, int maximum);
    void setValue(int lower, int upper);
    void setOrientation(Qt::Orientation orientation);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void drawRangeBackground(QStylePainter* painter, QStyleOptionSlider& opt1, QStyleOptionSlider& opt2);
    virtual void initStyleOption(QStyleOptionSlider& option, bool first) const;
};