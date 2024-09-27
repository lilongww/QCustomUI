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
    bool invertedAppearance { false };
    int singleStep { 1 };
    int pageStep { 10 };
    QStyle::SubControl pressedControl;
    QStyle::SubControl hoverControl;
    Handle pressedHandle;
    Handle hoverHandle;
    QRect hoverRect;
    void init();
    inline void resetLayoutItemMargins()
    {
        Q_Q(QCtmRangeSlider);
        QStyleOptionSlider opt;
        // ### This is (also) reached from the ctor which is unfortunate since a possible
        // ### re-implementation of initStyleOption is then not called.
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

QCtmRangeSlider::QCtmRangeSlider(QWidget* parent /*= nullptr*/) : QWidget(*new QCtmRangeSliderPrivate, parent, {})
{
    Q_D(QCtmRangeSlider);
    d->init();
}

QCtmRangeSlider::~QCtmRangeSlider() {}

Qt::Orientation QCtmRangeSlider::orientation() const
{
    Q_D(const QCtmRangeSlider);
    return d->orientation;
}

void QCtmRangeSlider::setMinimum(int minimum)
{
    Q_D(QCtmRangeSlider);
    setRange(minimum, std::max(minimum, d->maximum));
}

int QCtmRangeSlider::minimum() const
{
    Q_D(const QCtmRangeSlider);
    return d->minimum;
}

void QCtmRangeSlider::setMaximum(int maximum)
{
    Q_D(QCtmRangeSlider);
    setRange(std::min(maximum, d->minimum), maximum);
}

int QCtmRangeSlider::maximum() const
{
    Q_D(const QCtmRangeSlider);
    return d->maximum;
}

void QCtmRangeSlider::setLower(int value)
{
    Q_D(QCtmRangeSlider);
    setValue(value, d->upper);
}

int QCtmRangeSlider::lower() const
{
    Q_D(const QCtmRangeSlider);
    return d->lower;
}

void QCtmRangeSlider::setUpper(int value)
{
    Q_D(QCtmRangeSlider);
    setValue(d->lower, value);
}

int QCtmRangeSlider::upper() const
{
    Q_D(const QCtmRangeSlider);
    return d->upper;
}

void QCtmRangeSlider::setTracking(bool on)
{
    Q_D(QCtmRangeSlider);
    d->tracking = on;
}

bool QCtmRangeSlider::hasTracking() const
{
    Q_D(const QCtmRangeSlider);
    return d->tracking;
}

void QCtmRangeSlider::setTickPosition(TickPosition pos)
{
    Q_D(QCtmRangeSlider);
    if (d->tickPosition == pos)
        return;
    d->tickPosition = pos;
    update();
    updateGeometry();
}

QCtmRangeSlider::TickPosition QCtmRangeSlider::tickPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->tickPosition;
}

void QCtmRangeSlider::setSliderPosition(int lower, int upper)
{
    Q_D(QCtmRangeSlider);
    std::tie(lower, upper) = std::minmax({ lower, upper });
    auto l                 = std::clamp(lower, d->minimum, d->maximum);
    auto u                 = std::clamp(upper, d->minimum, d->maximum);
    qDebug() << lower << ":" << upper;
    bool emitValueChanged = (l != d->lower || u != d->upper);

    d->lower = l;
    d->upper = u;
    if (d->lowerPosition != l || d->upperPosition != u)
    {
        d->lowerPosition = l;
        d->upperPosition = u;
        emit sliderMoved(d->lowerPosition, d->upperPosition);
    }
    if (d->tracking)
        setValue(d->lowerPosition, d->upperPosition);
}

int QCtmRangeSlider::lowerSliderPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->lowerPosition;
}

int QCtmRangeSlider::upperSliderPosition() const
{
    Q_D(const QCtmRangeSlider);
    return d->upperPosition;
}

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

void QCtmRangeSlider::setRange(int minimum, int maximum)
{
    Q_D(QCtmRangeSlider);
    if (d->minimum == minimum && d->maximum == maximum)
        return;
    std::tie(d->minimum, d->maximum) = std::minmax(minimum, maximum);

    emit rangeChanged(d->minimum, d->maximum);
    setValue(d->lower, d->upper);
    update();
}

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

void QCtmRangeSlider::paintEvent(QPaintEvent* event)
{
    Q_D(QCtmRangeSlider);

    QStylePainter p(this);
    QStyleOptionSlider opt1;
    QStyleOptionSlider opt2;
    initStyleOption(opt1, true);
    initStyleOption(opt2, false);
    opt1.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
    if (d->tickPosition != QSlider::NoTicks)
        opt1.subControls |= QStyle::SC_SliderTickmarks;

    p.drawComplexControl(QStyle::CC_Slider, opt1);
    opt2.subControls = QStyle::SC_SliderHandle;
    p.drawComplexControl(QStyle::CC_Slider, opt2);
}

bool QCtmRangeSlider::event(QEvent* event)
{
    Q_D(QCtmRangeSlider);

    switch (event->type())
    {
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        if (const QHoverEvent* he = static_cast<const QHoverEvent*>(event))
            d->updateHoverControl(he->position().toPoint());
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
        d->pixelPosToRangeValue(d->pick(event->position().toPoint()), d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower);
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
        d->updatePressedHandle(ev->position().toPoint());
        bool lower = d->pressedHandle == QCtmRangeSliderPrivate::Handle::Lower;
        initStyleOption(opt, lower);
        d->pressedControl = style()->hitTestComplexControl(QStyle::CC_Slider, &opt, ev->position().toPoint(), this);
        if (d->pressedControl == QStyle::SC_SliderGroove)
        {
            const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
            int pressValue = d->pixelPosToRangeValue(d->pick(ev->position().toPoint() - sliderRect.center() + sliderRect.topLeft()));
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
}

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
    option.upsideDown =
        (d->orientation == Qt::Horizontal) ? (d->invertedAppearance != (option.direction == Qt::RightToLeft)) : (!d->invertedAppearance);
    option.direction      = first ? Qt::LeftToRight : Qt::RightToLeft; // we use the upsideDown option instead
    option.sliderPosition = first ? d->lowerPosition : (d->maximum - d->upperPosition);
    option.sliderValue    = first ? d->lower : (d->maximum - d->upper);
    option.singleStep     = d->singleStep;
    option.pageStep       = d->pageStep;
    if (d->orientation == Qt::Horizontal)
        option.state |= QStyle::State_Horizontal;

    if (d->pressedControl)
    {
        option.activeSubControls = d->pressedControl;
        option.state |= QStyle::State_Sunken;
    }
    else
    {
        if ((first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Lower) ||
            (!first && d->hoverHandle == QCtmRangeSliderPrivate::Handle::Upper))
            option.activeSubControls = d->hoverControl;
    }
}
