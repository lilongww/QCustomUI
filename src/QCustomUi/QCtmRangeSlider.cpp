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
#include "QCtmRangeSlider.h"
#include "Private/QtVersionAdapter.h"

#include <QDebug>
#include <QStyleOption>
#include <QStylePainter>
#include <private/qapplication_p.h>
#include <private/qwidget_p.h>

#include <algorithm>

class QCtmRangeSliderPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QCtmRangeSlider)
public:
    enum class Handle
    {
        None,
        Lower,
        Upper
    };
    Qt::Orientation orientation { Qt::Horizontal };
    QCtmRangeSlider::TickPosition tickPosition { QCtmRangeSlider::TickPosition::NoTicks };
    int tickInterval { 1 };
    int minimum { 0 }, maximum { 99 };
    int lower { 0 }, upper { 0 };
    int lowerPosition { 0 }, upperPosition { 0 };
    bool tracking { true };
    bool invertedAppearance { true };
    QStyle::SubControl pressedControl;
    QStyle::SubControl hoverControl;
    Handle pressedHandle;
    Handle hoverHandle;
    QRect hoverRect;
    bool chunkVisible { false };
    void init();
    inline void resetLayoutItemMargins()
    {
        Q_Q(QCtmRangeSlider);
        QStyleOptionSlider opt;
        q->initStyleOption(opt, true);
        setLayoutItemMargins(QStyle::SE_SliderLayoutItem, &opt);
    }

    int pixelPosToRangeValue(int pos, bool lower = true) const
    {
        Q_Q(const QCtmRangeSlider);
        QStyleOptionSlider opt;
        q->initStyleOption(opt, lower);
        QRect gr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, q);
        QRect sr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, q);
        int sliderMin, sliderMax, sliderLength;

        if (orientation == Qt::Horizontal)
        {
            sliderLength = sr.width();
            sliderMin    = gr.x();
            sliderMax    = gr.right() - sliderLength + 1;
        }
        else
        {
            sliderLength = sr.height();
            sliderMin    = gr.y();
            sliderMax    = gr.bottom() - sliderLength + 1;
        }
        return QStyle::sliderValueFromPosition(minimum, maximum, pos - sliderMin, sliderMax - sliderMin, opt.upsideDown);
    }

    inline int pick(const QPoint& pt) const { return orientation == Qt::Horizontal ? pt.x() : pt.y(); }

    bool updateHoverControl(const QPoint& pos)
    {
        Q_Q(QCtmRangeSlider);
        QRect lastHoverRect                 = hoverRect;
        QStyle::SubControl lastHoverControl = hoverControl;
        bool doesHover                      = q->testAttribute(Qt::WA_Hover);
        if (lastHoverControl != newHoverControl(pos) && doesHover)
        {
            q->update(lastHoverRect);
            q->update(hoverRect);
            return true;
        }
        return !doesHover;
    }

    QStyle::SubControl newHoverControl(const QPoint& pos)
    {
        Q_Q(QCtmRangeSlider);
        QStyleOptionSlider opt1;
        QStyleOptionSlider opt2;

        q->initStyleOption(opt1, true);
        q->initStyleOption(opt2, false);

        opt1.subControls = QStyle::SC_All;
        opt2.subControls = QStyle::SC_All;

        QRect handleRect1   = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderHandle, q);
        QRect grooveRect    = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderGroove, q);
        QRect tickmarksRect = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderTickmarks, q);
        QRect handleRect2   = q->style()->subControlRect(QStyle::CC_Slider, &opt2, QStyle::SC_SliderHandle, q);

        if (handleRect1.contains(pos))
        {
            hoverHandle  = Handle::Lower;
            hoverControl = QStyle::SC_SliderHandle;
            hoverRect    = handleRect1;
        }
        else if (handleRect2.contains(pos))
        {
            hoverHandle  = Handle::Upper;
            hoverControl = QStyle::SC_SliderHandle;
            hoverRect    = handleRect2;
        }
        else if (grooveRect.contains(pos))
        {
            hoverRect    = grooveRect;
            hoverControl = QStyle::SC_SliderGroove;
            hoverHandle  = (pos - handleRect1.center()).manhattanLength() <= (pos - handleRect2.center()).manhattanLength() ? Handle::Lower
                                                                                                                            : Handle::Upper;
        }
        else if (tickmarksRect.contains(pos))
        {
            hoverRect    = tickmarksRect;
            hoverControl = QStyle::SC_SliderTickmarks;
            hoverHandle  = (pos - handleRect1.center()).manhattanLength() <= (pos - handleRect2.center()).manhattanLength() ? Handle::Lower
                                                                                                                            : Handle::Upper;
        }
        else
        {
            hoverRect    = QRect();
            hoverControl = QStyle::SC_None;
            hoverHandle  = Handle::None;
        }

        return hoverControl;
    }

    inline void updatePressedHandle(const QPoint& pos)
    {
        Q_Q(QCtmRangeSlider);
        QStyleOptionSlider opt1;
        QStyleOptionSlider opt2;

        q->initStyleOption(opt1, true);
        q->initStyleOption(opt2, false);

        opt1.subControls = QStyle::SC_All;
        opt2.subControls = QStyle::SC_All;

        QRect handleRect1   = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderHandle, q);
        QRect grooveRect    = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderGroove, q);
        QRect tickmarksRect = q->style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SC_SliderTickmarks, q);
        QRect handleRect2   = q->style()->subControlRect(QStyle::CC_Slider, &opt2, QStyle::SC_SliderHandle, q);

        if (handleRect1.contains(pos))
        {
            pressedHandle = Handle::Lower;
        }
        else if (handleRect2.contains(pos))
        {
            pressedHandle = Handle::Upper;
        }
        else if (grooveRect.contains(pos))
        {
            pressedHandle = (pos - handleRect1.center()).manhattanLength() <= (pos - handleRect2.center()).manhattanLength()
                                ? Handle::Lower
                                : Handle::Upper;
        }
        else if (tickmarksRect.contains(pos))
        {
            pressedHandle = (pos - handleRect1.center()).manhattanLength() <= (pos - handleRect2.center()).manhattanLength()
                                ? Handle::Lower
                                : Handle::Upper;
        }
        else
        {
            pressedHandle = Handle::None;
        }
    }
};

void QCtmRangeSliderPrivate::init()
{
    Q_Q(QCtmRangeSlider);
    q->setAttribute(Qt::WA_Hover);
    pressedControl = QStyle::SC_None;
    tickInterval   = 0;
    tickPosition   = QSlider::NoTicks;
    hoverControl   = QStyle::SC_None;
    q->setFocusPolicy(Qt::FocusPolicy(q->style()->styleHint(QStyle::SH_Button_FocusPolicy)));
    QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::Slider);
    if (orientation == Qt::Vertical)
        sp.transpose();
    q->setSizePolicy(sp);
    q->setAttribute(Qt::WA_WState_OwnSizePolicy, false);
    resetLayoutItemMargins();
}

/*!
    \class      QCtmRangeSlider
    \brief      范围选择器.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmRangeSlider.h
*/

/*!
    \fn         void QCtmRangeSlider::valueChanged(int lower, int upper);
    \brief      部件上下限发生变化时发送该信号, \a lower, \a upper.
*/

/*!
    \fn         void QCtmRangeSlider::rangeChanged(int minimum, int maximum);
    \brief      部件范围发生变化时发送该信号, \a minimum, \a maximum.
*/

/*!
    \fn         void QCtmRangeSlider::sliderMoved(int lower, int upper);
    \brief      部件滑块发生变化时发送该信号, \a lower, \a upper.
*/

/*!
    \property   QCtmRangeSlider::tracking
    \brief      表示valueChange 信号是否跟踪拖拽发送。
    \sa         setTracking, hasTracking
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmRangeSlider::QCtmRangeSlider(QWidget* parent /*= nullptr*/) : QWidget(*new QCtmRangeSliderPrivate, parent, {})
{
    Q_D(QCtmRangeSlider);
    d->init();
}

/*!
    \brief      构造函数 \a orientation, \a parent.
*/
QCtmRangeSlider::QCtmRangeSlider(Qt::Orientation orientation, QWidget* parent /*= nullptr*/) : QCtmRangeSlider(parent)
{
    Q_D(QCtmRangeSlider);
    setOrientation(orientation);
}

/*!
    \brief      析构函数.
*/
QCtmRangeSlider::~QCtmRangeSlider() {}

/*!
    \brief      返回该部件的朝向.
    \sa         setOrientation
*/
Qt::Orientation QCtmRangeSlider::orientation() const
{
    Q_D(const QCtmRangeSlider);
    return d->orientation;
}

/*!
    \brief      设置最小值 \a minimum.
    \sa         minimum
*/
void QCtmRangeSlider::setMinimum(int minimum)
{
    Q_D(QCtmRangeSlider);
    setRange(minimum, std::max(minimum, d->maximum));
}

/*!
    \brief      返回最小值.
    \sa         setMinimum
*/
int QCtmRangeSlider::minimum() const
{
    Q_D(const QCtmRangeSlider);
    return d->minimum;
}

/*!
    \brief      设置最大值 \a maximum.
    \sa         maximum
*/
void QCtmRangeSlider::setMaximum(int maximum)
{
    Q_D(QCtmRangeSlider);
    setRange(std::min(maximum, d->minimum), maximum);
}

/*!
    \brief      返回最大值.
    \sa         setMaximum
*/
int QCtmRangeSlider::maximum() const
{
    Q_D(const QCtmRangeSlider);
    return d->maximum;
}

/*!
    \brief      设置范围下限 \a value.
    \sa         lower
*/
void QCtmRangeSlider::setLower(int value)
{
    Q_D(QCtmRangeSlider);
    setValue(value, d->upper);
}

/*!
    \brief      返回范围下限.
    \sa         setLower
*/
int QCtmRangeSlider::lower() const
{
    Q_D(const QCtmRangeSlider);
    return d->lower;
}

/*!
    \brief      设置范围上限 \a value.
    \sa         lower
*/
void QCtmRangeSlider::setUpper(int value)
{
    Q_D(QCtmRangeSlider);
    setValue(d->lower, value);
}

/*!
    \brief      返回范围上限.
    \sa         setUpper
*/
int QCtmRangeSlider::upper() const
{
    Q_D(const QCtmRangeSlider);
    return d->upper;
}

/*!
    \brief      设置 valueChange 信号是否跟踪拖拽发送 \a on.
    \sa         hasTracking
*/
void QCtmRangeSlider::setTracking(bool on)
{
    Q_D(QCtmRangeSlider);
    d->tracking = on;
}

/*!
    \brief      返回 valueChange 信号是否跟踪拖拽发送.
    \sa         setTracking
*/
bool QCtmRangeSlider::hasTracking() const
{
    Q_D(const QCtmRangeSlider);
    return d->tracking;
}

/*!
    \brief      设置刻度显示位置 \a pos.
    \sa         tickPosition
*/
void QCtmRangeSlider::setTickPosition(TickPosition pos)
{
    Q_D(QCtmRangeSlider);
    if (d->tickPosition == pos)
        return;
    d->tickPosition = pos;
    update();
    updateGeometry();
}

/*!
    \brief      返回刻度显示位置.
    \sa         setTickPosition
*/
QCtmRangeSlider::TickPosition QCtmRangeSlider::tickPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->tickPosition;
}

/*!
    \brief      设置滑块位置 \a lower, \a upper.
    \sa         lowerSliderPosition, upperSliderPosition
*/
void QCtmRangeSlider::setSliderPosition(int lower, int upper)
{
    Q_D(QCtmRangeSlider);
    std::tie(lower, upper) = std::minmax({ lower, upper });
    auto l                 = std::clamp(lower, d->minimum, d->maximum);
    auto u                 = std::clamp(upper, d->minimum, d->maximum);

    if (d->lowerPosition != l || d->upperPosition != u)
    {
        d->lowerPosition = l;
        d->upperPosition = u;
        emit sliderMoved(d->lowerPosition, d->upperPosition);
    }
    if (d->tracking)
        setValue(d->lowerPosition, d->upperPosition);
}

/*!
    \brief      返回下限滑块位置.
    \sa         setSliderPosition
*/
int QCtmRangeSlider::lowerSliderPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->lowerPosition;
}

/*!
    \brief      返回上限滑块位置.
    \sa         setSliderPosition
*/
int QCtmRangeSlider::upperSliderPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->upperPosition;
}

/*!
    \reimp
*/
QSize QCtmRangeSlider::sizeHint() const
{
    Q_D(const QCtmRangeSlider);
    ensurePolished();
    const int SliderLength = 84, TickSpace = 5;
    QStyleOptionSlider opt;
    initStyleOption(opt, true);
    int thick = style()->pixelMetric(QStyle::PM_SliderThickness, &opt, this);
    if (d->tickPosition & TickPosition::TicksAbove)
        thick += TickSpace;
    if (d->tickPosition & TickPosition::TicksBelow)
        thick += TickSpace;
    int w = thick, h = SliderLength;
    if (d->orientation == Qt::Horizontal)
    {
        w = SliderLength;
        h = thick;
    }
    return style()->sizeFromContents(QStyle::CT_Slider, &opt, QSize(w, h), this);
}

/*!
    \reimp
*/
QSize QCtmRangeSlider::minimumSizeHint() const
{
    Q_D(const QCtmRangeSlider);
    QSize s = sizeHint();
    QStyleOptionSlider opt;
    initStyleOption(opt, true);
    int length = style()->pixelMetric(QStyle::PM_SliderLength, &opt, this);
    if (d->orientation == Qt::Horizontal)
        s.setWidth(length);
    else
        s.setHeight(length);
    return s;
}

/*!
    \brief      设置滑块之间的区块是否显示，若要对其设置样式表，则先设置为显示 \a visible.
    \sa         chunkVisible
*/
void QCtmRangeSlider::setChunkVisible(bool visible)
{
    Q_D(QCtmRangeSlider);
    d->chunkVisible = visible;
    update();
}

/*!
    \brief      返回滑块之间的区块是否显示.
    \sa         setChunkVisible
*/
bool QCtmRangeSlider::chunkVisible() const
{
    Q_D(const QCtmRangeSlider);
    return d->chunkVisible;
}

/*!
    \brief      设置最大值和最小值 \a minimum, \a maximum.
    \sa         setMinimum, setMaximum
*/
void QCtmRangeSlider::setRange(int minimum, int maximum)
{
    Q_D(QCtmRangeSlider);
    if (d->minimum == minimum && d->maximum == maximum)
        return;
    std::tie(d->minimum, d->maximum) = std::minmax(minimum, maximum);

    setValue(d->lower, d->upper);
    emit rangeChanged(d->minimum, d->maximum);
    update();
}

/*!
    \brief      设置上下限 \a lower, \a upper.
    \sa         setLower, setUpper
*/
void QCtmRangeSlider::setValue(int lower, int upper)
{
    Q_D(QCtmRangeSlider);
    auto l = std::clamp(std::min(lower, upper), d->minimum, d->maximum);
    auto u = std::clamp(std::max(upper, lower), d->minimum, d->maximum);

    bool emitValueChanged = (l != d->lower || u != d->upper);

    d->lower = l;
    d->upper = u;
    if (d->lowerPosition != l || d->upperPosition != u)
    {
        d->lowerPosition = l;
        d->upperPosition = u;
        emit sliderMoved(d->lowerPosition, d->upperPosition);
    }
    if (emitValueChanged)
        emit valueChanged(d->lower, d->upper);
    update();
}

/*!
    \brief      设置部件方向 \a orientation.
    \sa         orientation
*/
void QCtmRangeSlider::setOrientation(Qt::Orientation orientation)
{
    Q_D(QCtmRangeSlider);
    if (orientation == d->orientation)
        return;
    d->orientation = orientation;
    if (!testAttribute(Qt::WA_WState_OwnSizePolicy))
    {
        setSizePolicy(sizePolicy().transposed());
        setAttribute(Qt::WA_WState_OwnSizePolicy, false);
    }
    update();
    updateGeometry();
}

/*!
    \reimp
*/
void QCtmRangeSlider::paintEvent(QPaintEvent* event)
{
    Q_D(QCtmRangeSlider);

    QStylePainter p(this);
    QStyleOptionSlider opt1;
    QStyleOptionSlider opt2;
    initStyleOption(opt1, true);
    initStyleOption(opt2, false);

    opt1.subControls = QStyle::SC_SliderGroove;
    if (d->tickPosition != QSlider::NoTicks)
    {
        opt1.subControls |= QStyle::SC_SliderTickmarks;
    }
    p.drawComplexControl(QStyle::CC_Slider, opt1);
    opt1.subControls = QStyle::SC_SliderHandle;
    opt2.subControls = QStyle::SC_SliderHandle;
    if (d->chunkVisible)
        drawRangeBackground(&p, opt1, opt2);
    p.drawComplexControl(QStyle::CC_Slider, opt1);
    p.drawComplexControl(QStyle::CC_Slider, opt2);
}

/*!
    \reimp
*/
bool QCtmRangeSlider::event(QEvent* event)
{
    Q_D(QCtmRangeSlider);

    switch (event->type())
    {
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        if (const QHoverEvent* he = static_cast<const QHoverEvent*>(event))
            d->updateHoverControl(QtVersionAdapter::eventPos(he));
        break;
    case QEvent::StyleChange:
    case QEvent::MacSizeChange:
        d->resetLayoutItemMargins();
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

/*!
    \reimp
*/
void QCtmRangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QCtmRangeSlider);
    if (d->pressedControl != QStyle::SC_SliderHandle)
    {
        event->ignore();
        return;
    }
    event->accept();
    int newPosition =
        d->pixelPosToRangeValue(d->pick(QtVersionAdapter::eventPos(event)), d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower);
    if (d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower)
    {
        auto [l, u] = std::minmax(newPosition, d->upperPosition);
        if (l != newPosition)
            d->pressedHandle = QCtmRangeSliderPrivate::Handle::Upper;
        setSliderPosition(l, u);
    }
    else
    {
        auto [l, u] = std::minmax(d->lowerPosition, newPosition);
        if (l != d->lowerPosition)
            d->pressedHandle = QCtmRangeSliderPrivate::Handle::Lower;
        setSliderPosition(l, u);
    }
}

/*!
    \reimp
*/
void QCtmRangeSlider::mousePressEvent(QMouseEvent* ev)
{
    Q_D(QCtmRangeSlider);
    if (d->maximum == d->minimum || (ev->buttons() ^ ev->button()))
    {
        ev->ignore();
        return;
    }

    ev->accept();
    if ((ev->button() & style()->styleHint(QStyle::SH_Slider_PageSetButtons)) == ev->button())
    {
        QStyleOptionSlider opt;
        d->updatePressedHandle(QtVersionAdapter::eventPos(ev));
        bool lower = d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower;
        initStyleOption(opt, lower);
        d->pressedControl = style()->hitTestComplexControl(QStyle::CC_Slider, &opt, QtVersionAdapter::eventPos(ev), this);
        if (d->pressedControl == QStyle::SC_SliderGroove)
        {
            const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
            int pressValue = d->pixelPosToRangeValue(d->pick(QtVersionAdapter::eventPos(ev) - sliderRect.center() + sliderRect.topLeft()));
            if (lower)
            {
                setSliderPosition(pressValue, d->upperPosition);
            }
            else
            {
                setSliderPosition(d->lowerPosition, pressValue);
                if (d->lowerPosition > pressValue)
                    d->pressedHandle = QCtmRangeSliderPrivate::Handle::Lower;
            }
            d->pressedControl = QStyle::SC_SliderHandle;
        }
    }
    else
    {
        ev->ignore();
        return;
    }
}

/*!
    \reimp
*/
void QCtmRangeSlider::mouseReleaseEvent(QMouseEvent* ev)
{
    Q_D(QCtmRangeSlider);
    if (d->pressedControl == QStyle::SC_None || ev->buttons())
    {
        ev->ignore();
        return;
    }
    ev->accept();
    QStyle::SubControl oldPressed = QStyle::SubControl(d->pressedControl);
    d->pressedControl             = QStyle::SC_None;
    QStyleOptionSlider opt;
    initStyleOption(opt, d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower);
    opt.subControls = oldPressed;
    update(style()->subControlRect(QStyle::CC_Slider, &opt, oldPressed, this));
    if (!d->tracking)
    {
        setValue(d->lowerPosition, d->upperPosition);
    }
}

/*!
    \brief      绘制滑块之间的区块, \a painter, \a opt1, \a opt2.
*/
void QCtmRangeSlider::drawRangeBackground(QStylePainter* painter, QStyleOptionSlider& opt1, QStyleOptionSlider& opt2)
{
    Q_D(QCtmRangeSlider);
    auto p1         = style()->subControlRect(QStyle::CC_Slider, &opt1, QStyle::SubControl::SC_SliderHandle, this).center();
    auto p2         = style()->subControlRect(QStyle::CC_Slider, &opt2, QStyle::SubControl::SC_SliderHandle, this).center();
    auto grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt2, QStyle::SubControl::SC_SliderGroove, this);

    QRect rangeRect;
    if (d->orientation == Qt::Horizontal)
        rangeRect = QRect(p1.x(), grooveRect.top(), std::abs(p2.x() - p1.x()), grooveRect.height());
    else
        rangeRect = QRect(grooveRect.left(), p1.y(), grooveRect.width(), std::abs(p1.y() - p2.y()));

    QStyleOptionProgressBar chunk;
    chunk.initFrom(this);
    chunk.minimum  = 0;
    chunk.maximum  = 100;
    chunk.progress = 100;
    chunk.rect     = rangeRect;

    painter->drawPrimitive(QStyle::PE_IndicatorProgressChunk, chunk);
}

/*!
    \brief      初始化样式选项 \a option, \a first.
*/
void QCtmRangeSlider::initStyleOption(QStyleOptionSlider& option, bool first) const
{
    Q_D(const QCtmRangeSlider);

    option.initFrom(this);
    option.subControls       = QStyle::SC_None;
    option.activeSubControls = QStyle::SC_None;
    option.orientation       = d->orientation;
    option.maximum           = d->maximum;
    option.minimum           = d->minimum;
    option.tickPosition      = d->tickPosition;
    option.tickInterval      = d->tickInterval;
    option.upsideDown        = false;
    //(d->orientation == Qt::Horizontal) ? (d->invertedAppearance != (option.direction == Qt::RightToLeft)) : (!d->invertedAppearance);
    option.direction      = Qt::LeftToRight;
    option.sliderPosition = first ? d->lowerPosition : d->upperPosition;
    option.sliderValue    = first ? d->lower : d->upper;
    option.singleStep     = 1;
    option.pageStep       = 10;
    if (d->orientation == Qt::Horizontal)
        option.state |= QStyle::State_Horizontal;

    if (d->pressedControl)
    {
        if ((first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Lower) ||
            (!first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Upper))
        {
            option.activeSubControls = d->pressedControl;
            option.state |= QStyle::State_Sunken;
        }
    }
    else
    {
        if ((first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Lower) ||
            (!first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Upper))
            option.activeSubControls = d->hoverControl;
    }
}
