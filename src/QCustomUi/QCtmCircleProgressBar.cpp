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
#include "QCtmCircleProgressBar.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QVariantAnimation>

struct QCtmCircleProgressBar::Impl
{
    int minimum { 0 };
    int maximum { 100 };
    int value { 24 };
    bool textVisible { true };
    QString textFormat { "%p%" };
    QVariantAnimation ani;
    QVariantAnimation rangeAni;
    int barWidth { 10 };
    QBrush barBrush { 0x326CF3 };
    QBrush backgroundBrush { 0xEEEEEE };
    Qt::PenCapStyle penCapStyle { Qt::FlatCap };
    inline double persent() const { return minimum == maximum ? 1.00 : (static_cast<double>(value) - minimum) / (maximum - minimum); }
    inline int arc() const { return -360 * 16 * persent(); }
    inline void startAni()
    {
        if (minimum == maximum && !minimum)
        {
            ani.setStartValue(360 * 16);
            ani.setEndValue(-360 * 16);
            ani.setDuration(2000);
            ani.start();
            ani.setLoopCount(-1);
            rangeAni.start();
        }
        else
        {
            rangeAni.stop();
            ani.setDuration(100);
            ani.setStartValue(ani.endValue());
            ani.setEndValue(arc());
            ani.setLoopCount(1);
            ani.start();
        }
    }
};

/*!
    \class      QCtmCircleProgressBar
    \brief      圆形进度条.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmCircleProgressBar.h
*/

/*!
    \property   QCtmCircleProgressBar::value
    \brief      当前进度值.
*/

/*!
    \property   QCtmCircleProgressBar::maximum
    \brief      最大值.
*/

/*!
    \property   QCtmCircleProgressBar::minimum
    \brief      最小值.
*/

/*!
    \property   QCtmCircleProgressBar::format
    \brief      文本格式, 参考 QProgressBar.
*/

/*!
    \property   QCtmCircleProgressBar::textVisible
    \brief      文本是否可见.
*/

/*!
    \property   QCtmCircleProgressBar::text
    \brief      进度条文本.
*/

/*!
    \property   QCtmCircleProgressBar::penCapStyle
    \brief      画笔端点样式.
*/

/*!
    \property   QCtmCircleProgressBar::barWidth
    \brief      进度条宽度.
*/

/*!
    \property   QCtmCircleProgressBar::barBrush
    \brief      进度条前景色画刷.
*/

/*!
    \property   QCtmCircleProgressBar::backgroundBrush
    \brief      进度条背景色画刷.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmCircleProgressBar::QCtmCircleProgressBar(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->ani.setStartValue(m_impl->arc());
    m_impl->ani.setEndValue(m_impl->arc());
    m_impl->rangeAni.setStartValue(-270 * 16);
    m_impl->rangeAni.setKeyValueAt(0.5, 0);
    m_impl->rangeAni.setEndValue(-270 * 16);
    m_impl->rangeAni.setDuration(2000);
    m_impl->rangeAni.setLoopCount(-1);
    connect(&m_impl->ani, &QVariantAnimation::valueChanged, this, qOverload<void>(&QWidget::update));
}

/*!
    \brief      析构函数.
*/
QCtmCircleProgressBar::~QCtmCircleProgressBar() {}

/*!
    \brief      返回当前值.
    \sa         setValue
*/
int QCtmCircleProgressBar::value() const { return m_impl->value; }

/*!
    \brief      返回最大值.
    \sa         setMaximum, setRange
*/
int QCtmCircleProgressBar::maximum() const { return m_impl->maximum; }

/*!
    \brief      返回最小值.
    \sa         setMinimum, setRange
*/
int QCtmCircleProgressBar::minimum() const { return m_impl->minimum; }

/*!
    \brief      设置文本格式 \a format, 参考 QProgressBar 文本格式.
    \sa         format
*/
void QCtmCircleProgressBar::setFormat(const QString& format)
{
    m_impl->textFormat = format;
    update();
}

/*!
    \brief      返回文本格式.
    \sa         setFormat
*/
const QString& QCtmCircleProgressBar::format() const { return m_impl->textFormat; }

/*!
    \brief      设置文本是否可见 \a visible.
    \sa         isTextVisible
*/
void QCtmCircleProgressBar::setTextVisible(bool visible)
{
    m_impl->textVisible = visible;
    update();
}

/*!
    \brief      返回文本是否可见.
    \sa         setTextVisible
*/
bool QCtmCircleProgressBar::isTextVisible() const { return m_impl->textVisible; }

/*!
    \brief      返回画笔样式.
    \sa         setPenCapStyle
*/
Qt::PenCapStyle QCtmCircleProgressBar::penCapStyle() const { return m_impl->penCapStyle; }

/*!
    \brief      返回进度条宽度.
    \sa         setBarWidth
*/
int QCtmCircleProgressBar::barWidth() const { return m_impl->barWidth; }

/*!
    \brief      返回进度条画刷.
    \sa         setBarBrush
*/
const QBrush& QCtmCircleProgressBar::barBrush() const { return m_impl->barBrush; }

/*!
    \brief      返回背景画刷.
    \sa         setBackgroundBrush
*/
const QBrush& QCtmCircleProgressBar::backgroundBrush() const { return m_impl->backgroundBrush; }

/*!
    \brief      返回文本字符串.
*/
QString QCtmCircleProgressBar::text() const
{
    if (m_impl->value < m_impl->minimum || m_impl->value > m_impl->maximum)
        return {};
    return QString(m_impl->textFormat)
        .replace("%p", QString::number(static_cast<int>(m_impl->persent() * 100.0)))
        .replace("%v", QString::number(m_impl->value))
        .replace("%m", QString::number(m_impl->maximum - m_impl->minimum));
}

/*!
    \brief      设置值 \a value.
    \sa         value
*/
void QCtmCircleProgressBar::setValue(int value)
{
    if (value == m_impl->value)
        return;
    m_impl->value = value;
    emit valueChanged(value);
    m_impl->startAni();
}

/*!
    \brief      设置最大值 \a maximum.
    \sa         maximum, setRange
*/
void QCtmCircleProgressBar::setMaximum(int maximum)
{
    if (maximum == m_impl->maximum)
        return;
    m_impl->maximum = maximum;
    m_impl->startAni();
}

/*!
    \brief      设置最小值 \a minimum.
    \sa         minimum, setRange
*/
void QCtmCircleProgressBar::setMinimum(int minimum)
{
    if (m_impl->minimum == minimum)
        return;
    m_impl->minimum = minimum;
    m_impl->startAni();
}

/*!
    \brief      设置值范围 \a minimum, \a maximum.
    \sa         setMaximum, setMinimum
*/
void QCtmCircleProgressBar::setRange(int minimum, int maximum)
{
    if (m_impl->maximum == maximum && m_impl->minimum == minimum)
        return;
    m_impl->maximum = maximum;
    m_impl->minimum = minimum;
    m_impl->startAni();
}

/*!
    \brief      设置画笔样式 \a style.
    \sa         penCapStyle
*/
void QCtmCircleProgressBar::setPenCapStyle(Qt::PenCapStyle style)
{
    m_impl->penCapStyle = style;
    update();
}

/*!
    \brief      设置进度条宽度 \a width.
    \sa         barWidth
*/
void QCtmCircleProgressBar::setBarWidth(int width)
{
    m_impl->barWidth = width;
    update();
}

/*!
    \brief      设置进度条画刷 \a brush.
    \sa         barBrush
*/
void QCtmCircleProgressBar::setBarBrush(const QBrush& brush)
{
    m_impl->barBrush = brush;
    update();
}

/*!
    \brief      设置背景画刷 \a brush.
    \sa         backgroundBrush
*/
void QCtmCircleProgressBar::setBackgroundBrush(const QBrush& brush)
{
    m_impl->backgroundBrush = brush;
    update();
}

/*!
    \reimp
*/
void QCtmCircleProgressBar::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    auto diameter = std::min(width(), height());
    auto rect     = QRectF(m_impl->barWidth / 2.0 + (width() - diameter) / 2,
                       m_impl->barWidth / 2.0 + (height() - diameter) / 2,
                       diameter - m_impl->barWidth,
                       diameter - m_impl->barWidth);
    { // draw background
        QPen pen;
        pen.setWidth(m_impl->barWidth);
        pen.setBrush(m_impl->backgroundBrush);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawArc(rect, 0, 360 * 16);
    }
    if (m_impl->maximum)
    {
        if (m_impl->value < m_impl->minimum || m_impl->value > m_impl->maximum)
            return;
        { // draw bar
            QPen pen;
            pen.setWidth(m_impl->barWidth);
            pen.setBrush(m_impl->barBrush);
            pen.setCapStyle(m_impl->penCapStyle);
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawArc(rect, -270 * 16, m_impl->ani.currentValue().toInt());
        }

        if (m_impl->textVisible)
        { // draw text
            p.setPen(m_impl->barBrush.color());
            p.setFont(this->font());
            QTextOption textOpt;
            textOpt.setAlignment(Qt::AlignCenter);
            p.drawText(rect, text(), textOpt);
        }
    }
    else
    { // draw loop
        QPen pen;
        pen.setWidth(m_impl->barWidth);
        pen.setColor(m_impl->barBrush.color());
        pen.setCapStyle(m_impl->penCapStyle);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        auto range = m_impl->rangeAni.currentValue().toInt();
        p.drawArc(rect, m_impl->ani.currentValue().toInt() - range / 2, range);
    }
}

/*!
    \reimp
*/
QSize QCtmCircleProgressBar::sizeHint() const { return QSize(100, 100); }

/*!
    \reimp
*/
QSize QCtmCircleProgressBar::minimumSizeHint() const { return QSize(40, 40); }
