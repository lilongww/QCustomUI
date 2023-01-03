/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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

#include <QWidget>

class QCUSTOMUI_EXPORT QCtmCircleProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(QString format READ format WRITE setFormat)
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)
    Q_PROPERTY(QString text READ text CONSTANT)
    Q_PROPERTY(Qt::PenCapStyle penCapStyle READ penCapStyle WRITE setPenCapStyle)
    Q_PROPERTY(int barWidth READ barWidth WRITE setBarWidth)
    Q_PROPERTY(QBrush barBrush READ barBrush WRITE setBarBrush)
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
public:
    QCtmCircleProgressBar(QWidget* parent);
    ~QCtmCircleProgressBar();

    int value() const;
    int maximum() const;
    int minimum() const;
    void setFormat(const QString& format);
    const QString& format() const;
    void setTextVisible(bool visible);
    bool isTextVisible() const;
    Qt::PenCapStyle penCapStyle() const;
    int barWidth() const;
    const QBrush& barBrush() const;
    const QBrush& backgroundBrush() const;
    virtual QString text() const;
public slots:
    void setValue(int value);
    void setMaximum(int maximum);
    void setMinimum(int minimum);
    void setRange(int minimum, int maximum);
    void setPenCapStyle(Qt::PenCapStyle style);
    void setBarWidth(int width);
    void setBarBrush(const QBrush& brush);
    void setBackgroundBrush(const QBrush& brush);
signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
