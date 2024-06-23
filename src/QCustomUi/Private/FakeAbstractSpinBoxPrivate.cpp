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

#include <QAccessibleValueChangeEvent>
#include <private/qabstractspinbox_p.h>
#include <private/qdatetimeparser_p.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QAbstractSpinBoxPrivate::QAbstractSpinBoxPrivate()
    : edit(nullptr)
    , type(QMetaType::UnknownType)
    , spinClickTimerId(-1)
    , spinClickTimerInterval(100)
    , spinClickThresholdTimerId(-1)
    , spinClickThresholdTimerInterval(-1)
    , effectiveSpinRepeatRate(1)
    , buttonState(None)
    , cachedText(QLatin1String("\x01"))
    , cachedState(QValidator::Invalid)
    , pendingEmit(false)
    , readOnly(false)
    , wrapping(false)
    , ignoreCursorPositionChanged(false)
    , frame(true)
    , accelerate(false)
    , keyboardTracking(true)
    , cleared(false)
    , ignoreUpdateEdit(false)
    , correctionMode(QAbstractSpinBox::CorrectToPreviousValue)
    , stepModifier(Qt::ControlModifier)
    , acceleration(0)
    , hoverControl(QStyle::SC_None)
    , buttonSymbols(QAbstractSpinBox::UpDownArrows)
    , validator(nullptr)
    , showGroupSeparator(0)
    , wheelDeltaRemainder(0)
{
}
#else
QAbstractSpinBoxPrivate::QAbstractSpinBoxPrivate()
    : pendingEmit(false)
    , readOnly(false)
    , wrapping(false)
    , ignoreCursorPositionChanged(false)
    , frame(true)
    , accelerate(false)
    , keyboardTracking(true)
    , cleared(false)
    , ignoreUpdateEdit(false)
    , showGroupSeparator(false)
{
}
#endif
/*
   \internal
   Called when the QAbstractSpinBoxPrivate is destroyed
*/
QAbstractSpinBoxPrivate::~QAbstractSpinBoxPrivate() {}

/*!
    \internal
    Updates the old and new hover control. Does nothing if the hover
    control has not changed.
*/
bool QAbstractSpinBoxPrivate::updateHoverControl(const QPoint& pos)
{
    Q_Q(QAbstractSpinBox);
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

/*!
    \internal
    Returns the hover control at \a pos.
    This will update the hoverRect and hoverControl.
*/
QStyle::SubControl QAbstractSpinBoxPrivate::newHoverControl(const QPoint& pos)
{
    Q_Q(QAbstractSpinBox);

    QStyleOptionSpinBox opt;
    q->initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;
    hoverControl    = q->style()->hitTestComplexControl(QStyle::CC_SpinBox, &opt, pos, q);
    hoverRect       = q->style()->subControlRect(QStyle::CC_SpinBox, &opt, hoverControl, q);
    return hoverControl;
}

/*!
    \internal
    Strips any prefix/suffix from \a text.
*/

QString QAbstractSpinBoxPrivate::stripped(const QString& t, int* pos) const
{
    QStringView text(t);
    if (specialValueText.size() == 0 || text != specialValueText)
    {
        int from     = 0;
        int size     = text.size();
        bool changed = false;
        if (prefix.size() && text.startsWith(prefix))
        {
            from += prefix.size();
            size -= from;
            changed = true;
        }
        if (suffix.size() && text.endsWith(suffix))
        {
            size -= suffix.size();
            changed = true;
        }
        if (changed)
            text = text.mid(from, size);
    }

    const int s = text.size();
    text        = text.trimmed();
    if (pos)
        (*pos) -= (s - text.size());
    return text.toString();
}

void QAbstractSpinBoxPrivate::updateEditFieldGeometry()
{
    Q_Q(QAbstractSpinBox);
    QStyleOptionSpinBox opt;
    q->initStyleOption(&opt);
    opt.subControls = QStyle::SC_SpinBoxEditField;
    edit->setGeometry(q->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField, q));
}
/*!
    \internal
    Returns \c true if a specialValueText has been set and the current value is minimum.
*/

bool QAbstractSpinBoxPrivate::specialValue() const { return (value == minimum && !specialValueText.isEmpty()); }

/*!
    \internal Virtual function that emits signals when the value
    changes. Reimplemented in the different subclasses.
*/

void QAbstractSpinBoxPrivate::emitSignals(EmitPolicy, const QVariant&) {}

/*!
    \internal

    Slot connected to the line edit's textChanged(const QString &)
    signal.
*/

#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
void QAbstractSpinBoxPrivate::_q_editorTextChanged(const QString& t)
#else
void QAbstractSpinBoxPrivate::editorTextChanged(const QString& t)
#endif
{
    Q_Q(QAbstractSpinBox);

    if (keyboardTracking)
    {
        QString tmp             = t;
        int pos                 = edit->cursorPosition();
        QValidator::State state = q->validate(tmp, pos);
        if (state == QValidator::Acceptable)
        {
            const QVariant v = valueFromText(tmp);
            setValue(v, EmitIfChanged, tmp != t);
            pendingEmit = false;
        }
        else
        {
            pendingEmit = true;
        }
    }
    else
    {
        pendingEmit = true;
    }
}

/*!
    \internal

    Virtual slot connected to the line edit's
    cursorPositionChanged(int, int) signal. Will move the cursor to a
    valid position if the new one is invalid. E.g. inside the prefix.
    Reimplemented in Q[Date|Time|DateTime]EditPrivate to account for
    the different sections etc.
*/

#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
void QAbstractSpinBoxPrivate::_q_editorCursorPositionChanged(int oldpos, int newpos)
#else
void QAbstractSpinBoxPrivate::editorCursorPositionChanged(int oldpos, int newpos)
#endif
{
    if (!edit->hasSelectedText() && !ignoreCursorPositionChanged && !specialValue())
    {
        ignoreCursorPositionChanged = true;

        bool allowSelection = true;
        int pos             = -1;
        if (newpos < prefix.size() && newpos != 0)
        {
            if (oldpos == 0)
            {
                allowSelection = false;
                pos            = prefix.size();
            }
            else
            {
                pos = oldpos;
            }
        }
        else if (newpos > edit->text().size() - suffix.size() && newpos != edit->text().size())
        {
            if (oldpos == edit->text().size())
            {
                pos            = edit->text().size() - suffix.size();
                allowSelection = false;
            }
            else
            {
                pos = edit->text().size();
            }
        }
        if (pos != -1)
        {
            const int selSize =
                edit->selectionStart() >= 0 && allowSelection ? (edit->selectedText().size() * (newpos < pos ? -1 : 1)) - newpos + pos : 0;

            const QSignalBlocker blocker(edit);
            if (selSize != 0)
            {
                edit->setSelection(pos - selSize, selSize);
            }
            else
            {
                edit->setCursorPosition(pos);
            }
        }
        ignoreCursorPositionChanged = false;
    }
}

/*!
    \internal

    Initialises the QAbstractSpinBoxPrivate object.
*/

void QAbstractSpinBoxPrivate::init()
{
    Q_Q(QAbstractSpinBox);

    q->setLineEdit(new QLineEdit(q));
    edit->setObjectName("qt_spinbox_lineedit");
    validator = new QSpinBoxValidator(q, this);
    edit->setValidator(validator);

    QStyleOptionSpinBox opt;
    // ### This is called from the ctor and thus we shouldn't call initStyleOption yet
    // ### as we only call the base class implementation of initStyleOption called.
    q->initStyleOption(&opt);
    spinClickTimerInterval          = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatRate, &opt, q);
    spinClickThresholdTimerInterval = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatThreshold, &opt, q);
    q->setFocusPolicy(Qt::WheelFocus);
    q->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::SpinBox));
    q->setAttribute(Qt::WA_InputMethodEnabled);

    q->setAttribute(Qt::WA_MacShowFocusRect);
}

/*!
    \internal

    Resets the state of the spinbox. E.g. the state is set to
    (Keyboard|Up) if Key up is currently pressed.
*/

void QAbstractSpinBoxPrivate::reset()
{
    Q_Q(QAbstractSpinBox);

    buttonState = None;
    if (q)
    {
        if (spinClickTimerId != -1)
            q->killTimer(spinClickTimerId);
        if (spinClickThresholdTimerId != -1)
            q->killTimer(spinClickThresholdTimerId);
        spinClickTimerId = spinClickThresholdTimerId = -1;
        acceleration                                 = 0;
        q->update();
    }
}

/*!
    \internal

    Updates the state of the spinbox.
*/

void QAbstractSpinBoxPrivate::updateState(bool up, bool fromKeyboard /* = false */)
{
    Q_Q(QAbstractSpinBox);
    if ((up && (buttonState & Up)) || (!up && (buttonState & Down)))
        return;
    reset();
    if (q && (q->stepEnabled() & (up ? QAbstractSpinBox::StepUpEnabled : QAbstractSpinBox::StepDownEnabled)))
    {
        buttonState = (up ? Up : Down) | (fromKeyboard ? Keyboard : Mouse);
        int steps   = up ? 1 : -1;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if (QGuiApplication::keyboardModifiers() & stepModifier)
#else
        if (keyboardModifiers & stepModifier)
#endif
            steps *= 10;
        q->stepBy(steps);
        spinClickThresholdTimerId = q->startTimer(spinClickThresholdTimerInterval);
#if QT_CONFIG(accessibility)
        QAccessibleValueChangeEvent event(q, value);
        QAccessible::updateAccessibility(&event);
#endif
    }
}

/*!
    \internal

    Bounds \a val to be within minimum and maximum. Also tries to be
    clever about setting it at min and max depending on what it was
    and what direction it was changed etc.
*/

QVariant QAbstractSpinBoxPrivate::bound(const QVariant& val, const QVariant& old, int steps) const
{
    QVariant v = val;
    if (!wrapping || steps == 0 || old.isNull())
    {
        if (variantCompare(v, minimum) < 0)
        {
            v = wrapping ? maximum : minimum;
        }
        if (variantCompare(v, maximum) > 0)
        {
            v = wrapping ? minimum : maximum;
        }
    }
    else
    {
        const bool wasMin  = old == minimum;
        const bool wasMax  = old == maximum;
        const int oldcmp   = variantCompare(v, old);
        const int maxcmp   = variantCompare(v, maximum);
        const int mincmp   = variantCompare(v, minimum);
        const bool wrapped = (oldcmp > 0 && steps < 0) || (oldcmp < 0 && steps > 0);
        if (maxcmp > 0)
        {
            v = ((wasMax && !wrapped && steps > 0) || (steps < 0 && !wasMin && wrapped)) ? minimum : maximum;
        }
        else if (wrapped && (maxcmp > 0 || mincmp < 0))
        {
            v = ((wasMax && steps > 0) || (!wasMin && steps < 0)) ? minimum : maximum;
        }
        else if (mincmp < 0)
        {
            v = (!wasMax && !wasMin ? minimum : maximum);
        }
    }

    return v;
}

/*!
    \internal

    Sets the value of the spin box to \a val. Depending on the value
    of \a ep it will also emit signals.
*/

void QAbstractSpinBoxPrivate::setValue(const QVariant& val, EmitPolicy ep, bool doUpdate)
{
    Q_Q(QAbstractSpinBox);
    const QVariant old = value;
    value              = bound(val);
    pendingEmit        = false;
    cleared            = false;
    if (doUpdate)
    {
        updateEdit();
    }
    q->update();

    if (ep == AlwaysEmit || (ep == EmitIfChanged && old != value))
    {
        emitSignals(ep, old);
    }
}

/*!
    \internal

    Updates the line edit to reflect the current value of the spin box.
*/

void QAbstractSpinBoxPrivate::updateEdit()
{
    Q_Q(QAbstractSpinBox);
    if (type == QMetaType::UnknownType)
        return;
    const QString newText = specialValue() ? specialValueText : prefix + textFromValue(value) + suffix;
    if (newText == edit->displayText() || cleared)
        return;

    const bool empty = edit->text().isEmpty();
    int cursor       = edit->cursorPosition();
    int selsize      = edit->selectedText().size();
    const QSignalBlocker blocker(edit);
    edit->setText(newText);

    if (!specialValue())
    {
        cursor = qBound(prefix.size(), cursor, edit->displayText().size() - suffix.size());

        if (selsize > 0)
        {
            edit->setSelection(cursor, selsize);
        }
        else
        {
            edit->setCursorPosition(empty ? prefix.size() : cursor);
        }
    }
    q->update();
}

/*!
    \internal

    Convenience function to set min/max values.
*/

void QAbstractSpinBoxPrivate::setRange(const QVariant& min, const QVariant& max)
{
    Q_Q(QAbstractSpinBox);

    clearCache();
    minimum               = min;
    maximum               = (variantCompare(min, max) < 0 ? max : min);
    cachedSizeHint        = QSize();
    cachedMinimumSizeHint = QSize(); // minimumSizeHint cares about min/max

    reset();
    if (!(bound(value) == value))
    {
        setValue(bound(value), EmitIfChanged);
    }
    else if (value == minimum && !specialValueText.isEmpty())
    {
        updateEdit();
    }

    q->updateGeometry();
}

/*!
    \internal

    Convenience function to get a variant of the right type.
*/

QVariant QAbstractSpinBoxPrivate::getZeroVariant() const
{
    QVariant ret;
    switch (type)
    {
    case QMetaType::Int:
        ret = QVariant(0);
        break;
    case QMetaType::Double:
        ret = QVariant(0.0);
        break;
    case QMetaType::LongLong:
        ret = QVariant(0LL);
        break;
    case QMetaType::ULongLong:
        ret = QVariant(0ULL);
        break;
    default:
        break;
    }
    return ret;
}

/*!
    \internal

    Virtual method called that calls the public textFromValue()
    functions in the subclasses. Needed to change signature from
    QVariant to int/double/QDateTime etc. Used when needing to display
    a value textually.

    This method is reimeplemented in the various subclasses.
*/

QString QAbstractSpinBoxPrivate::textFromValue(const QVariant&) const { return QString(); }

/*!
    \internal

    Virtual method called that calls the public valueFromText()
    functions in the subclasses. Needed to change signature from
    QVariant to int/double/QDateTime etc. Used when needing to
    interpret a string as another type.

    This method is reimeplemented in the various subclasses.
*/

QVariant QAbstractSpinBoxPrivate::valueFromText(const QString&) const { return QVariant(); }
/*!
    \internal

    Interprets text and emits signals. Called when the spinbox needs
    to interpret the text on the lineedit.
*/

void QAbstractSpinBoxPrivate::interpret(EmitPolicy ep)
{
    Q_Q(QAbstractSpinBox);
    if (type == QMetaType::UnknownType || cleared)
        return;

    QVariant v       = getZeroVariant();
    bool doInterpret = true;
    QString tmp      = edit->displayText();
    int pos          = edit->cursorPosition();
    const int oldpos = pos;

    if (q->validate(tmp, pos) != QValidator::Acceptable)
    {
        const QString copy = tmp;
        q->fixup(tmp);
        // QASBDEBUG() << "QAbstractSpinBoxPrivate::interpret() text '" << edit->displayText() << "' >> '" << copy << '\'' << "' >> '" <<
        // tmp
        //             << '\'';

        doInterpret = tmp != copy && (q->validate(tmp, pos) == QValidator::Acceptable);
        if (!doInterpret)
        {
            v = (correctionMode == QAbstractSpinBox::CorrectToNearestValue ? variantBound(minimum, v, maximum) : value);
        }
    }
    if (doInterpret)
    {
        v = valueFromText(tmp);
    }
    clearCache();
    setValue(v, ep, true);
    if (oldpos != pos)
        edit->setCursorPosition(pos);
}

void QAbstractSpinBoxPrivate::clearCache() const
{
    cachedText.clear();
    cachedValue.clear();
    cachedState = QValidator::Acceptable;
}

QVariant QAbstractSpinBoxPrivate::calculateAdaptiveDecimalStep(int steps) const
{
    Q_UNUSED(steps);
    return singleStep;
}

// --- QSpinBoxValidator ---

/*!
    \internal
    Constructs a QSpinBoxValidator object
*/

QSpinBoxValidator::QSpinBoxValidator(QAbstractSpinBox* qp, QAbstractSpinBoxPrivate* dp) : QValidator(qp), qptr(qp), dptr(dp)
{
    setObjectName("qt_spinboxvalidator");
}

/*!
    \internal

    Checks for specialValueText, prefix, suffix and calls
    the virtual QAbstractSpinBox::validate function.
*/

QValidator::State QSpinBoxValidator::validate(QString& input, int& pos) const
{
    if (dptr->specialValueText.size() > 0 && input == dptr->specialValueText)
        return QValidator::Acceptable;

    if (!dptr->prefix.isEmpty() && !input.startsWith(dptr->prefix))
    {
        input.prepend(dptr->prefix);
        pos += dptr->prefix.size();
    }

    if (!dptr->suffix.isEmpty() && !input.endsWith(dptr->suffix))
        input.append(dptr->suffix);

    return qptr->validate(input, pos);
}
/*!
    \internal
    Calls the virtual QAbstractSpinBox::fixup function.
*/

void QSpinBoxValidator::fixup(QString& input) const { qptr->fixup(input); }

// --- global ---

/*!
    \internal
    Adds two variants together and returns the result.
*/

QVariant operator+(const QVariant& arg1, const QVariant& arg2)
{
    QVariant ret;
    if (Q_UNLIKELY(arg1.userType() != arg2.userType()))
        qWarning(
            "QAbstractSpinBox: Internal error: Different types (%s vs %s) (%s:%d)", arg1.typeName(), arg2.typeName(), __FILE__, __LINE__);
    switch (arg1.userType())
    {
    case QMetaType::Int:
        {
            const int int1 = arg1.toInt();
            const int int2 = arg2.toInt();
            if (int1 > 0 && (int2 >= INT_MAX - int1))
            {
                // The increment overflows
                ret = QVariant(INT_MAX);
            }
            else if (int1 < 0 && (int2 <= INT_MIN - int1))
            {
                // The increment underflows
                ret = QVariant(INT_MIN);
            }
            else
            {
                ret = QVariant(int1 + int2);
            }
            break;
        }
    case QMetaType::LongLong:
        {
            const auto int1 = arg1.toLongLong();
            const auto int2 = arg2.toLongLong();
            if (int1 > 0 && (int2 >= std::numeric_limits<qlonglong>::max() - int1))
            {
                // The increment overflows
                ret = QVariant(std::numeric_limits<qlonglong>::max());
            }
            else if (int1 < 0 && (int2 <= std::numeric_limits<qlonglong>::min() - int1))
            {
                // The increment underflows
                ret = QVariant(std::numeric_limits<qlonglong>::min());
            }
            else
            {
                ret = QVariant(int1 + int2);
            }
            break;
        }
    case QMetaType::ULongLong:
        {
            const auto int1 = arg1.toULongLong();
            const auto int2 = arg2.toULongLong();
            if (int1 > 0 && (int2 >= std::numeric_limits<qulonglong>::max() - int1))
            {
                // The increment overflows
                ret = QVariant(std::numeric_limits<qulonglong>::max());
            }
            else if (int1 < 0 && (int2 <= std::numeric_limits<qulonglong>::min() - int1))
            {
                // The increment underflows
                ret = QVariant(std::numeric_limits<qulonglong>::min());
            }
            else
            {
                ret = QVariant(int1 + int2);
            }
            break;
        }
    case QMetaType::Double:
        ret = QVariant(arg1.toDouble() + arg2.toDouble());
        break;
#if QT_CONFIG(datetimeparser)
    case QMetaType::QDateTime:
        {
            QDateTime a2 = arg2.toDateTime();
            QDateTime a1 = arg1.toDateTime().addDays(QDATETIMEEDIT_DATE_MIN.daysTo(a2.date()));
            a1.setTime(a1.time().addMSecs(a2.time().msecsSinceStartOfDay()));
            ret = QVariant(a1);
            break;
        }
#endif // datetimeparser
    default:
        break;
    }
    return ret;
}

/*!
    \internal
    Subtracts two variants and returns the result.
*/

QVariant operator-(const QVariant& arg1, const QVariant& arg2)
{
    QVariant ret;
    if (Q_UNLIKELY(arg1.userType() != arg2.userType()))
        qWarning(
            "QAbstractSpinBox: Internal error: Different types (%s vs %s) (%s:%d)", arg1.typeName(), arg2.typeName(), __FILE__, __LINE__);
    switch (arg1.userType())
    {
    case QMetaType::Int:
        ret = QVariant(arg1.toInt() - arg2.toInt());
        break;
    case QMetaType::LongLong:
        ret = QVariant(arg1.toLongLong() - arg2.toLongLong());
        break;
    case QMetaType::ULongLong:
        ret = QVariant(arg1.toULongLong() - arg2.toULongLong());
        break;
    case QMetaType::Double:
        ret = QVariant(arg1.toDouble() - arg2.toDouble());
        break;
    case QMetaType::QDateTime:
        {
            QDateTime a1 = arg1.toDateTime();
            QDateTime a2 = arg2.toDateTime();
            int days     = a2.daysTo(a1);
            int secs     = a2.secsTo(a1);
            int msecs    = qMax(0, a1.time().msec() - a2.time().msec());
            if (days < 0 || secs < 0 || msecs < 0)
            {
                ret = arg1;
            }
            else
            {
                QDateTime dt = a2.addDays(days).addSecs(secs);
                if (msecs > 0)
                    dt.setTime(dt.time().addMSecs(msecs));
                ret = QVariant(dt);
            }
            break;
        }
    default:
        break;
    }
    return ret;
}

/*!
    \internal
    Multiplies \a arg1 by \a multiplier and returns the result.
*/

QVariant operator*(const QVariant& arg1, double multiplier)
{
    QVariant ret;

    switch (arg1.userType())
    {
    case QMetaType::Int:
        ret = static_cast<int>(qBound<double>(INT_MIN, arg1.toInt() * multiplier, INT_MAX));
        break;
    case QMetaType::LongLong:
        ret = static_cast<qlonglong>(
            qBound<double>(std::numeric_limits<qlonglong>::min(), arg1.toLongLong() * multiplier, std::numeric_limits<qlonglong>::max()));
        break;
    case QMetaType::ULongLong:
        ret = static_cast<qulonglong>(qBound<double>(
            std::numeric_limits<qulonglong>::min(), arg1.toULongLong() * multiplier, std::numeric_limits<qulonglong>::max()));
        break;
    case QMetaType::Double:
        ret = QVariant(arg1.toDouble() * multiplier);
        break;
#if QT_CONFIG(datetimeparser)
    case QMetaType::QDateTime:
        {
            double days          = QDATETIMEEDIT_DATE_MIN.daysTo(arg1.toDateTime().date()) * multiplier;
            const qint64 daysInt = qint64(days);
            days -= daysInt;
            qint64 msecs = qint64(arg1.toDateTime().time().msecsSinceStartOfDay() * multiplier + days * (24 * 3600 * 1000));
            ret          = QDATETIMEEDIT_DATE_MIN.addDays(daysInt).startOfDay().addMSecs(msecs);
            break;
        }
#endif // datetimeparser
    default:
        ret = arg1;
        break;
    }

    return ret;
}

double operator/(const QVariant& arg1, const QVariant& arg2)
{
    double a1 = 0;
    double a2 = 0;

    switch (arg1.userType())
    {
    case QMetaType::Int:
        a1 = (double)arg1.toInt();
        a2 = (double)arg2.toInt();
        break;
    case QMetaType::LongLong:
        a1 = (double)arg1.toLongLong();
        a2 = (double)arg2.toLongLong();
        break;
    case QMetaType::ULongLong:
        a1 = (double)arg1.toULongLong();
        a2 = (double)arg2.toULongLong();
        break;
    case QMetaType::Double:
        a1 = arg1.toDouble();
        a2 = arg2.toDouble();
        break;
#if QT_CONFIG(datetimeparser)
    case QMetaType::QDateTime:
        a1 = QDATETIMEEDIT_DATE_MIN.daysTo(arg1.toDate());
        a2 = QDATETIMEEDIT_DATE_MIN.daysTo(arg2.toDate());
        a1 += arg1.toDateTime().time().msecsSinceStartOfDay() / (36e5 * 24);
        a2 += arg2.toDateTime().time().msecsSinceStartOfDay() / (36e5 * 24);
        break;
#endif // datetimeparser
    default:
        break;
    }

    return (a1 != 0 && a2 != 0) ? (a1 / a2) : 0.0;
}

int QAbstractSpinBoxPrivate::variantCompare(const QVariant& arg1, const QVariant& arg2)
{
    switch (arg2.userType())
    {
    case QMetaType::QDate:
        Q_ASSERT_X(arg1.userType() == QMetaType::QDate,
                   "QAbstractSpinBoxPrivate::variantCompare",
                   qPrintable(QString::fromLatin1("Internal error 1 (%1)").arg(QString::fromLatin1(arg1.typeName()))));
        if (arg1.toDate() == arg2.toDate())
        {
            return 0;
        }
        else if (arg1.toDate() < arg2.toDate())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::QTime:
        Q_ASSERT_X(arg1.userType() == QMetaType::QTime,
                   "QAbstractSpinBoxPrivate::variantCompare",
                   qPrintable(QString::fromLatin1("Internal error 2 (%1)").arg(QString::fromLatin1(arg1.typeName()))));
        if (arg1.toTime() == arg2.toTime())
        {
            return 0;
        }
        else if (arg1.toTime() < arg2.toTime())
        {
            return -1;
        }
        else
        {
            return 1;
        }

    case QMetaType::QDateTime:
        if (arg1.toDateTime() == arg2.toDateTime())
        {
            return 0;
        }
        else if (arg1.toDateTime() < arg2.toDateTime())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::Int:
        if (arg1.toInt() == arg2.toInt())
        {
            return 0;
        }
        else if (arg1.toInt() < arg2.toInt())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::Double:
        if (arg1.toDouble() == arg2.toDouble())
        {
            return 0;
        }
        else if (arg1.toDouble() < arg2.toDouble())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::LongLong:
        if (arg1.toLongLong() == arg2.toLongLong())
        {
            return 0;
        }
        else if (arg1.toLongLong() < arg2.toLongLong())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::ULongLong:
        if (arg1.toULongLong() == arg2.toULongLong())
        {
            return 0;
        }
        else if (arg1.toULongLong() < arg2.toULongLong())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    case QMetaType::UnknownType:
        if (arg2.userType() == QMetaType::UnknownType)
            return 0;
        Q_FALLTHROUGH();
    default:
        Q_ASSERT_X(0,
                   "QAbstractSpinBoxPrivate::variantCompare",
                   qPrintable(QString::fromLatin1("Internal error 3 (%1 %2)")
                                  .arg(QString::fromLatin1(arg1.typeName()), QString::fromLatin1(arg2.typeName()))));
    }
    return -2;
}

QVariant QAbstractSpinBoxPrivate::variantBound(const QVariant& min, const QVariant& value, const QVariant& max)
{
    Q_ASSERT(variantCompare(min, max) <= 0);
    if (variantCompare(min, value) < 0)
    {
        const int compMax = variantCompare(value, max);
        return (compMax < 0 ? value : max);
    }
    else
    {
        return min;
    }
}
