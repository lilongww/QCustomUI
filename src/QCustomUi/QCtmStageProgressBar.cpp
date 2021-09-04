/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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

#include <QPainter>
#include <QStyleOption>
#include <QPainterPath>
#include <QDebug>

struct QCtmStageProgressBar::Impl
{
    int stageCount{ 5 };
    Qt::Orientation orientation{ Qt::Horizontal };
    int minimum{ 0 };
    int maximum{ 99 };
    int value{ 0 };
    bool textVisible{ false };
    int stageCricleRadius{ 10 };
    std::vector<QString> texts{ static_cast<size_t>(stageCount) };
    QPen stageTextIndexColor{ Qt::white };
    QBrush rateBrush{ QColor{0x3580ce} };
};

/*!
    \class      QCtmStageProgressBar
    \brief      QCtmStageProgressBar provide a stage progress bar.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi

    \b          {The stage progress bar screenshot:}
    \image      QCtmStageProgressBarDetail.png
*/

/*!
    \brief      Constructs a progress bar with given \a parent widget.
*/
QCtmStageProgressBar::QCtmStageProgressBar(QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    setOrientation(Qt::Horizontal);
    adjustSize();
}

/*!
    \brief      Destroys the progress bar.
*/
QCtmStageProgressBar::~QCtmStageProgressBar()
{
}

/*!
    \brief      Set the \a orientation of progress bar.
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
    \brief      Returns the orientation of progress bar .
    \sa         setOrientation
*/
Qt::Orientation QCtmStageProgressBar::orientation() const
{
    return m_impl->orientation;
}

/*!
    \brief      Set \a count of the stage.
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
    \brief      Returns count of the stage.
    \sa         setStageCount
*/
int QCtmStageProgressBar::stageCount() const
{
    return m_impl->stageCount;
}

/*!
    \brief      Set the stage's circle \a radius pixel count.
    \sa         stageCricleRadius()
*/
void QCtmStageProgressBar::setStageCircleRadius(int radius)
{
    if (m_impl->stageCricleRadius == radius)
        return;
    m_impl->stageCricleRadius = radius;
    updateGeometry();
}

/*!
    \brief      Returns the stage's circle radius pixel count.
    \sa         setStageCircleRadius
*/
int QCtmStageProgressBar::stageCricleRadius() const
{
    return m_impl->stageCricleRadius;
}

/*!
    \brief      Set \a text of the \a stage.
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
    \brief      Returns text of the \a stage.
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
    \brief      Set the text is visible, \a flag.
    \sa         textVisible()
*/
void QCtmStageProgressBar::setTextVisible(bool flag)
{
    m_impl->textVisible = flag;
    updateGeometry();
}

/*!
    \brief      Returns current stage text is visible.
    \sa         setTextVisible
*/
bool QCtmStageProgressBar::textVisible() const
{
    return m_impl->textVisible;
}

/*!
    \brief      Set \a value of the progress bar.
    \sa         value()
*/
void QCtmStageProgressBar::setValue(int value)
{
    m_impl->value = value;
    normalize();
}

/*!
    \brief      Returns value of the progress bar.
    \sa         setValue
*/
int QCtmStageProgressBar::value() const
{
    return m_impl->value;
}

/*!
    \brief      Set \a maximum value the progress bar.
    \sa         maximum()
*/
void QCtmStageProgressBar::setMaximum(int maximum)
{
    if (m_impl->minimum > maximum)
        m_impl->minimum = maximum;
    m_impl->maximum = maximum;
    normalize();
}

/*!
    \brief      Returns maximum value of the progress bar.
    \sa         setMaximum
*/
int QCtmStageProgressBar::maximum() const
{
    return m_impl->maximum;
}

/*!
    \brief      Set minimum value the progress bar, \a min.
    \sa         minimum()
*/
void QCtmStageProgressBar::setMinimum(int min)
{
    if (m_impl->maximum < min)
        m_impl->maximum = min;
    m_impl->minimum = min;
    normalize();
}

/*!
    \brief      Returns  minimum value of the progress bar.
    \sa         setMinimum
*/
int QCtmStageProgressBar::minimum() const
{
    return m_impl->minimum;
}

/*!
    \brief      Set \a color of the text in stage circle.
    \sa         stageIndexTextColor()
*/
void QCtmStageProgressBar::setStageIndexTextColor(const QPen& color)
{
    m_impl->stageTextIndexColor = color;
    update();
}

/*!
    \brief      Returns color of the text in stage circle.
    \sa         setStageIndexTextColor
*/
const QPen& QCtmStageProgressBar::stageIndexTextColor() const
{
    return m_impl->stageTextIndexColor;
}

/*!
    \brief      Set background \a brush of the progress bar rate.
    \sa         rateBackground()
*/
void QCtmStageProgressBar::setRateBackground(const QBrush& brush)
{
    m_impl->rateBrush = brush;
    update();
}

/*!
    \brief      Returns  background brush of the progress bar rate.
    \sa         setRateBackground
*/
const QBrush& QCtmStageProgressBar::rateBackground() const
{
    return m_impl->rateBrush;
}

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
                channel.setTop(rect.center().y() - m_impl->stageCricleRadius / 4);
                channel.setBottom(rect.center().y() + m_impl->stageCricleRadius / 4);
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
                channel.setLeft(m_impl->textVisible ? rect.center().x() - m_impl->stageCricleRadius / 4 : rect.center().x() + m_impl->stageCricleRadius / 4);
                channel.setRight(m_impl->textVisible ? rect.center().x() + m_impl->stageCricleRadius / 4 : rect.center().x() - m_impl->stageCricleRadius / 4);
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
    auto v = static_cast<double>(m_impl->value) / m_impl->maximum;
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
    \brief      Draw pattern of the stage with given \a p, \a index, \a rect, \a value.
    \sa         drawText
*/
void QCtmStageProgressBar::drawStage([[maybe_unused]] QPainter* p, [[maybe_unused]] int index, [[maybe_unused]] const QRectF& rect, [[maybe_unused]] int value) const
{
    p->save();
    p->setPen(m_impl->stageTextIndexColor);
    p->drawText(rect, QString::number(index + 1), QTextOption(Qt::AlignCenter));
    p->restore();
}

/*!
    \brief      Draw texts with arguments \a p, \a index, \a rect, \a text.
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
QSize QCtmStageProgressBar::sizeHint() const
{
    return minimumSizeHint();
}

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
    \brief      Initialize the style option, \a opt.
*/
void QCtmStageProgressBar::initStyleOption(QStyleOptionProgressBar* opt) const
{
    if (!opt)
        return;
    opt->initFrom(this);

    if (m_impl->orientation == Qt::Horizontal)
        opt->state |= QStyle::State_Horizontal;
    opt->minimum = m_impl->minimum;
    opt->maximum = m_impl->maximum;
    opt->progress = m_impl->value;
    opt->textAlignment = Qt::AlignCenter;
    opt->textVisible = m_impl->textVisible;
}

/*!
    \brief      Calculate minimum width of the progress bar.
    \sa         doMinimumHeight()
*/
int QCtmStageProgressBar::doMinimumWidth() const
{
    if (Qt::Horizontal == m_impl->orientation)
    {
        auto step = (m_impl->stageCricleRadius * 2 * m_impl->stageCount + m_impl->stageCricleRadius * 3 * (m_impl->stageCount - 1) - m_impl->stageCricleRadius) / m_impl->stageCount;
        if (m_impl->textVisible)
        {
            int w{ 0 };
            auto beginLen{ 0 };
            auto endLen{ 0 };
            for (int i = 0; i < m_impl->texts.size(); i++)
            {
                const auto& text = m_impl->texts[i];
                auto len = fontMetrics().horizontalAdvance(text);
                if (w < len)
                    w = len;
                if (i == 0)
                    beginLen = len / 2 > m_impl->stageCricleRadius ? len / 2 - m_impl->stageCricleRadius : 0;
                if (i == m_impl->texts.size() - 1)
                    endLen = len / 2 > m_impl->stageCricleRadius ? len / 2 - m_impl->stageCricleRadius : 0;
            }
            auto progressPadding = std::max(beginLen, endLen);
            constexpr int space = 10;
            step = step > w + space ? step : (w + space);
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
            return len + m_impl->stageCricleRadius * 2 + 10;
        }
        else
        {
            return m_impl->stageCricleRadius * 2;
        }
    }
}

/*!
    \brief      Calculate minimum height of the progress bar.
    \sa         doMinimumWidth()
*/
int QCtmStageProgressBar::doMinimumHeight() const
{
    if (Qt::Horizontal == m_impl->orientation)
        return m_impl->stageCricleRadius * 2 + (m_impl->textVisible ? fontMetrics().height() + fontMetrics().leading() : 0);
    else
    {
        auto step = (m_impl->stageCricleRadius * 2 * m_impl->stageCount + m_impl->stageCricleRadius * 2 * (m_impl->stageCount - 1) - m_impl->stageCricleRadius) / m_impl->stageCount;

        int endLen = 0;
        int len = 0;
        for (int i = 0; i < m_impl->texts.size(); i++)
        {
            len = qMax(len, fontMetrics().size(0, m_impl->texts[i]).height());
            if (i == 0)
                endLen = len;
        }
        step = step < len + fontMetrics().leading() * 2 ? len + fontMetrics().leading() * 2 : step;
        return step * m_impl->stageCount + (endLen > m_impl->stageCricleRadius ? endLen - m_impl->stageCricleRadius : 0);
    }
}

/*!
    \brief      Returns rect of the stage by given \a index.
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
            auto step = (rect.width() - m_impl->stageCricleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(index * step + rect.x(), rect.top() + rect.height() / 2 - m_impl->stageCricleRadius, m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
        }
        else
        {
            auto firstTextLen = fontMetrics().horizontalAdvance(*m_impl->texts.begin());
            auto beginLen = firstTextLen / 2 > m_impl->stageCricleRadius ? firstTextLen / 2 - m_impl->stageCricleRadius : 0;
            auto endTextLen = fontMetrics().horizontalAdvance(*m_impl->texts.rbegin());
            auto endLen = endTextLen / 2 > m_impl->stageCricleRadius ? endTextLen / 2 - m_impl->stageCricleRadius : 0;
            auto progressPadding = std::max(beginLen, endLen);
            auto step = (rect.width() - m_impl->stageCricleRadius * 2 - progressPadding * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(index * step + rect.x() + progressPadding, rect.top() + rect.height() / 2 - minimumSizeHint().height() / 2, m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
        }
    }
    else
    {
        if (!m_impl->textVisible)
        {
            auto step = (rect.height() - m_impl->stageCricleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(rect.x() + rect.width() / 2 - m_impl->stageCricleRadius, index * step + rect.x(), m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
        }
        else
        {
            auto endLen = fontMetrics().size(0, *m_impl->texts.begin()).height();
            auto step = (rect.height() - qMax(endLen, m_impl->stageCricleRadius) - m_impl->stageCricleRadius) / static_cast<double>(m_impl->stageCount - 1);
            return QRectF(rect.x(), rect.top() + step * (m_impl->stageCount - 1 - index), m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
        }
    }
}

/*!
    \brief      Returns rect of the text by given \a index.
    \sa         doStageRect
*/
QRectF QCtmStageProgressBar::doTextRect(int index) const
{
    const auto& size = fontMetrics().size(0, m_impl->texts[index]);
    const auto& stageRect = doStageRect(index);
    if (m_impl->orientation == Qt::Horizontal)
    {
        return QRectF(stageRect.center().x() - size.width() / 2
            , stageRect.bottom() + fontMetrics().leading()
            , size.width()
            , size.height());
    }
    else
    {
        return QRectF(stageRect.right() + 10, stageRect.top(), size.width(), qMax(size.height(), m_impl->stageCricleRadius * 2));
    }
}

void QCtmStageProgressBar::normalize()
{
    if (m_impl->value > m_impl->maximum)
        m_impl->value = m_impl->maximum;
    else if (m_impl->value < m_impl->minimum)
        m_impl->value = m_impl->minimum;
    update();
}
