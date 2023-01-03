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

#include "QCtmStageProgressBar.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

struct QCtmStageProgressBar::Impl
{
    int stageCount { 5 };
    Qt::Orientation orientation { Qt::Horizontal };
    int minimum { 0 };
    int maximum { 99 };
    int value { 0 };
    bool textVisible { false };
    int stageCircleRadius { 10 };
    std::vector<QString> texts { static_cast<size_t>(stageCount) };
    QPen stageTextIndexPen { Qt::white };
    QBrush rateBrush { QColor { 0x3580ce } };
};

/*!
    \class      QCtmStageProgressBar
    \brief      阶段进度条.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmStageProgressBar.h

    \b          {截图:}
    \image      QCtmStageProgressBarDetail.png
*/

/*!
    \fn         void QCtmStageProgressBar::valueChanged(int value)
    \brief      当值 \a value 变化时发送该信号.
    \sa         setValue
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmStageProgressBar::QCtmStageProgressBar(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setOrientation(Qt::Horizontal);
    adjustSize();
}

/*!
    \brief      析构函数.
*/
QCtmStageProgressBar::~QCtmStageProgressBar() {}

/*!
    \brief      设置进度条的朝向 \a orientation.
    \sa         orientation()
*/
void QCtmStageProgressBar::setOrientation(Qt::Orientation orientation)
{
    if (m_impl->orientation == orientation)
        return;
    m_impl->orientation = orientation;
    if (m_impl->orientation == Qt::Horizontal)
        setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    else
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
    adjustSize();
    updateGeometry();
}

/*!
    \brief      返回进度条的朝向.
    \sa         setOrientation
*/
Qt::Orientation QCtmStageProgressBar::orientation() const { return m_impl->orientation; }

/*!
    \brief      设置阶段数量 \a count.
    \sa         stageCount()
*/
void QCtmStageProgressBar::setStageCount(int count)
{
    if (count < 2)
    {
        qDebug() << "stage count must be greater than or equal to 2";
        return;
    }
    if (count == m_impl->stageCount)
        return;
    m_impl->stageCount = count;
    m_impl->texts.resize(count);
    updateGeometry();
}

/*!
    \brief      返回阶段数量.
    \sa         setStageCount
*/
int QCtmStageProgressBar::stageCount() const { return m_impl->stageCount; }

/*!
    \brief      设置阶段节点圆形的半径像素 \a radius.
    \sa         stageCircleRadius
*/
void QCtmStageProgressBar::setStageCircleRadius(int radius)
{
    if (m_impl->stageCircleRadius == radius)
        return;
    m_impl->stageCircleRadius = radius;
    updateGeometry();
}

/*!
    \brief      返回阶段节点圆形的半径像素.
    \sa         setStageCircleRadius
*/
int QCtmStageProgressBar::stageCircleRadius() const { return m_impl->stageCircleRadius; }

/*!
    \brief      设置阶段 \a stage 的文本 \a text.
    \sa         text()
*/
void QCtmStageProgressBar::setText(int stage, const QString& text)
{
    if (stage >= m_impl->stageCount)
    {
        qDebug() << "stage out of range";
        return;
    }
    m_impl->texts[stage] = text;
    updateGeometry();
}

/*!
    \brief      返回阶段 \a stage 的文本.
    \sa         setText
*/
QString QCtmStageProgressBar::text(int stage) const
{
    if (stage >= m_impl->stageCount)
    {
        qDebug() << "stage out of range";
        return {};
    }
    return m_impl->texts[stage];
}

/*!
    \brief      设置阶段文本是否可见 \a flag.
    \sa         textVisible()
*/
void QCtmStageProgressBar::setTextVisible(bool flag)
{
    if (flag == m_impl->textVisible)
        return;
    m_impl->textVisible = flag;
    updateGeometry();
}

/*!
    \brief      返回阶段文本是否可见.
    \sa         setTextVisible
*/
bool QCtmStageProgressBar::textVisible() const { return m_impl->textVisible; }

/*!
    \brief      设置当前进度值 \a value, \a value 为范围外则无效.
    \sa         value
*/
void QCtmStageProgressBar::setValue(int value)
{
    if (m_impl->value == value)
        return;
    if (value < m_impl->minimum || value > m_impl->maximum)
        return;
    m_impl->value = value;
    emit valueChanged(m_impl->value);
    repaint();
}

/*!
    \brief      返回当前进度.
    \sa         setValue
*/
int QCtmStageProgressBar::value() const { return m_impl->value; }

/*!
    \brief      设置进度条值范围 \a minimum, \a maximum.
    \sa         setMaximum, setMinimum
*/
void QCtmStageProgressBar::setRange(int minimum, int maximum)
{
    if (m_impl->minimum != minimum || m_impl->maximum != maximum)
    {
        m_impl->minimum = minimum;
        m_impl->maximum = std::max(maximum, minimum);
        setValue(std::clamp(m_impl->value, m_impl->minimum, m_impl->maximum));
        update();
    }
}

/*!
    \brief      设置最大值 \a maximum.
    \sa         maximum
*/
void QCtmStageProgressBar::setMaximum(int maximum) { setRange(std::min(m_impl->minimum, maximum), maximum); }

/*!
    \brief      返回最大值.
    \sa         setMaximum
*/
int QCtmStageProgressBar::maximum() const { return m_impl->maximum; }

/*!
    \brief      设置最小值 \a min.
    \sa         minimum
*/
void QCtmStageProgressBar::setMinimum(int min) { setRange(min, std::max(min, m_impl->maximum)); }

/*!
    \brief      返回最小值.
    \sa         setMinimum
*/
int QCtmStageProgressBar::minimum() const { return m_impl->minimum; }

/*!
    \brief      设置阶段序号文本的画笔 \a pen.
    \sa         stageIndexTextPen()
*/
void QCtmStageProgressBar::setStageIndexTextPen(const QPen& pen)
{
    m_impl->stageTextIndexPen = pen;
    update();
}

/*!
    \brief      返回阶段序号文本的画笔.
    \sa         setStageIndexTextPen
*/
const QPen& QCtmStageProgressBar::stageIndexTextPen() const { return m_impl->stageTextIndexPen; }

/*!
    \brief      设置进度条背景色 \a brush.
    \sa         rateBackground()
*/
void QCtmStageProgressBar::setRateBackground(const QBrush& brush)
{
    m_impl->rateBrush = brush;
    update();
}

/*!
    \brief      返回进度条背景色.
    \sa         setRateBackground
*/
const QBrush& QCtmStageProgressBar::rateBackground() const { return m_impl->rateBrush; }

/*!
    \reimp
*/
void QCtmStageProgressBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QRect channel;
    QPainterPath path;
    path.setFillRule(Qt::FillRule::WindingFill);

    for (int index = 0; index < m_impl->stageCount; index++)
    {
        const auto& rect = doStageRect(index);
        path.addEllipse(rect);
        if (m_impl->orientation == Qt::Horizontal)
        {
            if (index == 0)
            {
                channel.setLeft(rect.center().x());
                channel.setTop(rect.center().y() - m_impl->stageCircleRadius / 4);
                channel.setBottom(rect.center().y() + m_impl->stageCircleRadius / 4);
            }
            else if (index == m_impl->stageCount - 1)
            {
                channel.setRight(rect.center().x());
            }
        }
        else
        {
            if (index == 0)
            {
                channel.setLeft(m_impl->textVisible ? rect.center().x() - m_impl->stageCircleRadius / 4
                                                    : rect.center().x() + m_impl->stageCircleRadius / 4);
                channel.setRight(m_impl->textVisible ? rect.center().x() + m_impl->stageCircleRadius / 4
                                                     : rect.center().x() - m_impl->stageCircleRadius / 4);
                channel.setBottom(rect.center().y());
            }
            else if (index == m_impl->stageCount - 1)
            {
                channel.setTop(rect.center().y());
            }
        }
    }
    path.addRect(channel);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillPath(path, this->palette().window());

    auto&& boundRect = path.boundingRect();
    auto v           = static_cast<double>(m_impl->value) / m_impl->maximum;
    if (m_impl->orientation == Qt::Horizontal)
        boundRect.setWidth(boundRect.width() * v);
    else
        boundRect.setTop(boundRect.height() * (1 - v) + boundRect.top());
    p.save();
    p.setClipRect(boundRect);
    p.fillPath(path, m_impl->rateBrush);
    p.restore();
    for (int index = 0; index < m_impl->stageCount; index++)
    {
        const auto& rect = doStageRect(index);
        drawStage(&p, index, rect, m_impl->value);
        if (m_impl->textVisible)
            drawText(&p, index, doTextRect(index), m_impl->texts[index]);
    }
}

/*!
    \brief      绘制阶段序号文本 \a p, \a index, \a rect, \a value.
    \sa         drawText
*/
void QCtmStageProgressBar::drawStage(QPainter* p, int index, const QRectF& rect, [[maybe_unused]] int value) const
{
    p->save();
    p->setPen(m_impl->stageTextIndexPen);
    p->drawText(rect, QString::number(index + 1), QTextOption(Qt::AlignCenter));
    p->restore();
}

/*!
    \brief      绘制阶段文本 \a p, \a index, \a rect, \a text.
    \sa         drawStage
*/
void QCtmStageProgressBar::drawText(QPainter* p, [[maybe_unused]] int index, const QRectF& rect, const QString& text) const
{
    p->save();
    p->setPen(QPen(this->palette().windowText().color()));
    p->drawText(rect, text, QTextOption(m_impl->orientation == Qt::Horizontal ? Qt::AlignCenter : Qt::AlignVCenter | Qt::AlignLeft));
    p->restore();
}

/*!
    \reimp
*/
QSize QCtmStageProgressBar::sizeHint() const { return minimumSizeHint(); }

/*!
    \reimp
*/
QSize QCtmStageProgressBar::minimumSizeHint() const
{
    ensurePolished();
    QFontMetrics fm = fontMetrics();
    QStyleOptionProgressBar opt;
    initStyleOption(&opt);
    QSize size = QSize(doMinimumWidth(), doMinimumHeight());
    return style()->sizeFromContents(QStyle::CT_ProgressBar, &opt, size, this);
}

/*!
    \brief      初始化样式选项 \a opt.
*/
void QCtmStageProgressBar::initStyleOption(QStyleOptionProgressBar* opt) const
{
    if (!opt)
        return;
    opt->initFrom(this);

    if (m_impl->orientation == Qt::Horizontal)
        opt->state |= QStyle::State_Horizontal;
    opt->minimum       = m_impl->minimum;
    opt->maximum       = m_impl->maximum;
    opt->progress      = m_impl->value;
    opt->textAlignment = Qt::AlignCenter;
    opt->textVisible   = m_impl->textVisible;
}

/*!
    \brief      计算最小宽度.
    \sa         doMinimumHeight()
*/
int QCtmStageProgressBar::doMinimumWidth() const
{
    if (Qt::Horizontal == m_impl->orientation)
    {
        auto step = (m_impl->stageCircleRadius * 2 * m_impl->stageCount + m_impl->stageCircleRadius * 3 * (m_impl->stageCount - 1) -
                     m_impl->stageCircleRadius) /
                    m_impl->stageCount;
        if (m_impl->textVisible)
        {
            int w { 0 };
            auto beginLen { 0 };
            auto endLen { 0 };
            for (int i = 0; i < m_impl->texts.size(); i++)
            {
                const auto& text = m_impl->texts[i];
                auto len         = fontMetrics().horizontalAdvance(text);
                if (w < len)
                    w = len;
                if (i == 0)
                    beginLen = len / 2 > m_impl->stageCircleRadius ? len / 2 - m_impl->stageCircleRadius : 0;
                if (i == m_impl->texts.size() - 1)
                    endLen = len / 2 > m_impl->stageCircleRadius ? len / 2 - m_impl->stageCircleRadius : 0;
            }
            auto progressPadding = std::max(beginLen, endLen);
            constexpr int space  = 10;
            step                 = step > w + space ? step : (w + space);
            return step * (m_impl->stageCount - 2) + progressPadding * 2 + space;
        }
        else
            return step * m_impl->stageCount;
    }
    else
    {
        if (m_impl->textVisible)
        {
            int len = 0;
            for (const auto& text : m_impl->texts)
                len = qMax(len, fontMetrics().size(0, text).width());
            return len + m_impl->stageCircleRadius * 2 + 10;
        }
        else
        {
            return m_impl->stageCircleRadius * 2;
        }
    }
}

/*!
    \brief      计算最小高度.
    \sa         doMinimumWidth()
*/
int QCtmStageProgressBar::doMinimumHeight() const
{
    if (Qt::Horizontal == m_impl->orientation)
        return m_impl->stageCircleRadius * 2 + (m_impl->textVisible ? fontMetrics().height() + fontMetrics().leading() : 0);
    else
    {
        auto step = (m_impl->stageCircleRadius * 2 * m_impl->stageCount + m_impl->stageCircleRadius * 2 * (m_impl->stageCount - 1) -
                     m_impl->stageCircleRadius) /
                    m_impl->stageCount;

        int endLen = 0;
        int len    = 0;
        for (int i = 0; i < m_impl->texts.size(); i++)
        {
            len = qMax(len, fontMetrics().size(0, m_impl->texts[i]).height());
            if (i == 0)
                endLen = len;
        }
        step = step < len + fontMetrics().leading() * 2 ? len + fontMetrics().leading() * 2 : step;
        return step * m_impl->stageCount + (endLen > m_impl->stageCircleRadius ? endLen - m_impl->stageCircleRadius : 0);
    }
}

/*!
    \brief      计算阶段圆形包围盒 \a index.
    \sa         doTextRect
*/
QRectF QCtmStageProgressBar::doStageRect(int index) const
{
    QStyleOptionProgressBar opt;
    initStyleOption(&opt);

    auto rect = style()->subElementRect(QStyle::SE_ProgressBarContents, &opt, this);

    if (m_impl->orientation == Qt::Horizontal)
    {
        if (!m_impl->textVisible)
        {
            auto step = (rect.width() - m_impl->stageCircleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(index * step + rect.x(),
                          rect.top() + rect.height() / 2 - m_impl->stageCircleRadius,
                          m_impl->stageCircleRadius * 2,
                          m_impl->stageCircleRadius * 2);
        }
        else
        {
            auto firstTextLen    = fontMetrics().horizontalAdvance(*m_impl->texts.begin());
            auto beginLen        = firstTextLen / 2 > m_impl->stageCircleRadius ? firstTextLen / 2 - m_impl->stageCircleRadius : 0;
            auto endTextLen      = fontMetrics().horizontalAdvance(*m_impl->texts.rbegin());
            auto endLen          = endTextLen / 2 > m_impl->stageCircleRadius ? endTextLen / 2 - m_impl->stageCircleRadius : 0;
            auto progressPadding = std::max(beginLen, endLen);
            auto step = (rect.width() - m_impl->stageCircleRadius * 2 - progressPadding * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(index * step + rect.x() + progressPadding,
                          rect.top() + rect.height() / 2 - minimumSizeHint().height() / 2,
                          m_impl->stageCircleRadius * 2,
                          m_impl->stageCircleRadius * 2);
        }
    }
    else
    {
        if (!m_impl->textVisible)
        {
            auto step = (rect.height() - m_impl->stageCircleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(rect.x() + rect.width() / 2 - m_impl->stageCircleRadius,
                          index * step + rect.x(),
                          m_impl->stageCircleRadius * 2,
                          m_impl->stageCircleRadius * 2);
        }
        else
        {
            auto endLen = fontMetrics().size(0, *m_impl->texts.begin()).height();
            auto step   = (rect.height() - qMax(endLen, m_impl->stageCircleRadius) - m_impl->stageCircleRadius) /
                        static_cast<double>(m_impl->stageCount - 1);
            return QRectF(rect.x(),
                          rect.top() + step * (m_impl->stageCount - 1 - index),
                          m_impl->stageCircleRadius * 2,
                          m_impl->stageCircleRadius * 2);
        }
    }
}

/*!
    \brief      计算阶段文本包围盒 \a index.
    \sa         doStageRect
*/
QRectF QCtmStageProgressBar::doTextRect(int index) const
{
    const auto& size      = fontMetrics().size(0, m_impl->texts[index]);
    const auto& stageRect = doStageRect(index);
    if (m_impl->orientation == Qt::Horizontal)
    {
        return QRectF(stageRect.center().x() - size.width() / 2, stageRect.bottom() + fontMetrics().leading(), size.width(), size.height());
    }
    else
    {
        return QRectF(stageRect.right() + 10, stageRect.top(), size.width(), qMax(size.height(), m_impl->stageCircleRadius * 2));
    }
}
