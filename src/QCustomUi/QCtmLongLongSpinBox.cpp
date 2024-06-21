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
#include "QCtmLongLongSpinBox.h"

#include <QStyle>
#include <QStyleOption>
#include <private/qabstractspinbox_p.h>

class QCtmLongLongSpinBoxPrivate : public QAbstractSpinBoxPrivate
{
    Q_DECLARE_PUBLIC(QCtmLongLongSpinBox)
public:
    QCtmLongLongSpinBoxPrivate();
    void emitSignals(EmitPolicy ep, const QVariant&) override;

    virtual QVariant valueFromText(const QString& n) const override;
    virtual QString textFromValue(const QVariant& n) const override;
    QVariant validateAndInterpret(QString& input, int& pos, QValidator::State& state) const;

    inline void init()
    {
        Q_Q(QCtmLongLongSpinBox);
        q->setInputMethodHints(Qt::ImhDigitsOnly);
        setLayoutItemMargins(QStyle::SE_SpinBoxLayoutItem);
    }

    int displayIntegerBase;

    QVariant calculateAdaptiveDecimalStep(int steps) const override;
};

QCtmLongLongSpinBoxPrivate::QCtmLongLongSpinBoxPrivate()
{
    minimum            = 0LL;
    maximum            = 99LL;
    value              = minimum;
    displayIntegerBase = 10;
    singleStep         = 1LL;
    type               = QMetaType::LongLong;
}

void QCtmLongLongSpinBoxPrivate::emitSignals(EmitPolicy ep, const QVariant& old)
{
    Q_Q(QCtmLongLongSpinBox);
    if (ep != NeverEmit)
    {
        pendingEmit = false;
        if (ep == AlwaysEmit || value != old)
        {
            emit q->textChanged(edit->displayText());
            emit q->valueChanged(value.toLongLong());
        }
    }
}

QVariant QCtmLongLongSpinBoxPrivate::valueFromText(const QString& text) const
{
    Q_Q(const QCtmLongLongSpinBox);

    return QVariant(q->valueFromText(text));
}

QString QCtmLongLongSpinBoxPrivate::textFromValue(const QVariant& n) const
{
    Q_Q(const QCtmLongLongSpinBox);
    return q->textFromValue(n.toLongLong());
}

QVariant QCtmLongLongSpinBoxPrivate::validateAndInterpret(QString& input, int& pos, QValidator::State& state) const
{
    if (cachedText == input && !input.isEmpty())
    {
        state = cachedState;
        // QSBDEBUG() << "cachedText was '" << cachedText << "' state was " << state << " and value was " << cachedValue;

        return cachedValue;
    }
    const auto max = maximum.toLongLong();
    const auto min = minimum.toLongLong();

    QString copy = stripped(input, &pos);
    // QSBDEBUG() << "input" << input << "copy" << copy;
    state    = QValidator::Acceptable;
    auto num = min;

    if (max != min && (copy.isEmpty() || (min < 0 && copy == "-") || (max >= 0 && copy == "+")))
    {
        state = QValidator::Intermediate;
        // QSBDEBUG() << __FILE__ << __LINE__ << "num is set to" << num;
    }
    else if (copy.startsWith(u'-') && min >= 0)
    {
        state = QValidator::Invalid; // special-case -0 will be interpreted as 0 and thus not be invalid with a range from 0-100
    }
    else
    {
        bool ok = false;
        if (displayIntegerBase != 10)
        {
            num = copy.toLongLong(&ok, displayIntegerBase);
        }
        else
        {
            num = locale.toLongLong(copy, &ok);
            if (!ok && (max >= 1000 || min <= -1000))
            {
                const QString sep(locale.groupSeparator());
                const QString doubleSep = sep + sep;
                if (copy.contains(sep) && !copy.contains(doubleSep))
                {
                    QString copy2 = copy;
                    copy2.remove(sep);
                    num = locale.toLongLong(copy2, &ok);
                }
            }
        }
        // QSBDEBUG() << __FILE__ << __LINE__ << "num is set to" << num;
        if (!ok)
        {
            state = QValidator::Invalid;
        }
        else if (num >= min && num <= max)
        {
            state = QValidator::Acceptable;
        }
        else if (max == min)
        {
            state = QValidator::Invalid;
        }
        else
        {
            if ((num >= 0 && num > max) || (num < 0 && num < min))
            {
                state = QValidator::Invalid;
                // QSBDEBUG() << __FILE__ << __LINE__ << "state is set to Invalid";
            }
            else
            {
                state = QValidator::Intermediate;
                // QSBDEBUG() << __FILE__ << __LINE__ << "state is set to Intermediate";
            }
        }
    }
    if (state != QValidator::Acceptable)
        num = max > 0 ? min : max;
    input       = prefix + copy + suffix;
    cachedText  = input;
    cachedState = state;
    cachedValue = QVariant(num);

    // QSBDEBUG() << "cachedText is set to '" << cachedText << "' state is set to " << state << " and value is set to " << cachedValue;
    return cachedValue;
}

QVariant QCtmLongLongSpinBoxPrivate::calculateAdaptiveDecimalStep(int steps) const
{
    const auto intValue = value.toLongLong();
    const auto absValue = qAbs(intValue);

    if (absValue < 100LL)
        return 1LL;

    const bool valueNegative    = intValue < 0;
    const bool stepsNegative    = steps < 0;
    const auto signCompensation = (valueNegative == stepsNegative) ? 0LL : 1LL;

    const auto log = static_cast<qlonglong>(std::log10(absValue - signCompensation)) - 1LL;
    return static_cast<qlonglong>(std::pow(10, log));
}

QCtmLongLongSpinBox::QCtmLongLongSpinBox(QWidget* parent /*= nullptr*/) : QAbstractSpinBox(*new QCtmLongLongSpinBoxPrivate, parent)
{
    Q_D(QCtmLongLongSpinBox);
    d->init();
}

QCtmLongLongSpinBox::~QCtmLongLongSpinBox() {}

void QCtmLongLongSpinBox::setValue(qlonglong value)
{
    Q_D(QCtmLongLongSpinBox);
    d->setValue(QVariant(value), EmitIfChanged);
}

qlonglong QCtmLongLongSpinBox::value() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->value.toLongLong();
}

void QCtmLongLongSpinBox::setPrefix(const QString& prefix)
{
    Q_D(QCtmLongLongSpinBox);

    d->prefix = prefix;
    d->updateEdit();

    d->cachedSizeHint        = QSize();
    d->cachedMinimumSizeHint = QSize(); // minimumSizeHint cares about the prefix
    updateGeometry();
}

QString QCtmLongLongSpinBox::prefix() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->prefix;
}

void QCtmLongLongSpinBox::setSuffix(const QString& suffix)
{
    Q_D(QCtmLongLongSpinBox);

    d->suffix = suffix;
    d->updateEdit();

    d->cachedSizeHint = QSize();
    updateGeometry();
}

QString QCtmLongLongSpinBox::suffix() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->suffix;
}

QString QCtmLongLongSpinBox::cleanText() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->stripped(d->edit->displayText());
}

void QCtmLongLongSpinBox::setSingleStep(qlonglong value)
{
    Q_D(QCtmLongLongSpinBox);
    if (value >= 0LL)
    {
        d->singleStep = QVariant(value);
        d->updateEdit();
    }
}

qlonglong QCtmLongLongSpinBox::singleStep() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->singleStep.toLongLong();
}

void QCtmLongLongSpinBox::setMinimum(qlonglong minimum)
{
    Q_D(QCtmLongLongSpinBox);
    const QVariant m(minimum);
    d->setRange(m, (QCtmLongLongSpinBoxPrivate::variantCompare(d->maximum, m) > 0 ? d->maximum : m));
}

qlonglong QCtmLongLongSpinBox::minimum() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->minimum.toLongLong();
}

void QCtmLongLongSpinBox::setMaximum(qlonglong maximum)
{
    Q_D(QCtmLongLongSpinBox);
    const QVariant m(maximum);
    d->setRange((QCtmLongLongSpinBoxPrivate::variantCompare(d->minimum, m) < 0 ? d->minimum : m), m);
}

qlonglong QCtmLongLongSpinBox::maximum() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->maximum.toLongLong();
}

void QCtmLongLongSpinBox::setRange(qlonglong minimum, qlonglong maximum)
{
    Q_D(QCtmLongLongSpinBox);
    d->setRange(QVariant(minimum), QVariant(maximum));
}

void QCtmLongLongSpinBox::setStepType(QAbstractSpinBox::StepType stepType)
{
    Q_D(QCtmLongLongSpinBox);
    d->stepType = stepType;
}

QAbstractSpinBox::StepType QCtmLongLongSpinBox::stepType() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->stepType;
}

void QCtmLongLongSpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QCtmLongLongSpinBox);
    // Falls back to base 10 on invalid bases (like QString)
    if (Q_UNLIKELY(base < 2 || base > 36))
    {
        qWarning("QSpinBox::setDisplayIntegerBase: Invalid base (%d)", base);
        base = 10;
    }

    if (base != d->displayIntegerBase)
    {
        d->displayIntegerBase = base;
        d->updateEdit();
    }
}

int QCtmLongLongSpinBox::displayIntegerBase() const
{
    Q_D(const QCtmLongLongSpinBox);
    return d->displayIntegerBase;
}

QString QCtmLongLongSpinBox::textFromValue(qlonglong value) const
{
    Q_D(const QCtmLongLongSpinBox);
    QString str;

    if (d->displayIntegerBase != 10)
    {
        const auto prefix = value < 0 ? "-" : "";
        str               = prefix + QString::number(qAbs(value), d->displayIntegerBase);
    }
    else
    {
        str = locale().toString(value);
        if (!d->showGroupSeparator && (qAbs(value) >= 1000LL || value == std::numeric_limits<qlonglong>::lowest()))
        {
            str.remove(locale().groupSeparator());
        }
    }

    return str;
}

qlonglong QCtmLongLongSpinBox::valueFromText(const QString& text) const
{
    Q_D(const QCtmLongLongSpinBox);

    QString copy            = text;
    int pos                 = d->edit->cursorPosition();
    QValidator::State state = QValidator::Acceptable;
    return d->validateAndInterpret(copy, pos, state).toLongLong();
}

QValidator::State QCtmLongLongSpinBox::validate(QString& input, int& pos) const
{
    Q_D(const QCtmLongLongSpinBox);

    QValidator::State state;
    d->validateAndInterpret(input, pos, state);
    return state;
}

void QCtmLongLongSpinBox::fixup(QString& str) const
{
    if (!isGroupSeparatorShown())
        str.remove(locale().groupSeparator());
}

bool QCtmLongLongSpinBox::event(QEvent* event)
{
    Q_D(QCtmLongLongSpinBox);
    if (event->type() == QEvent::StyleChange
#ifdef Q_OS_MAC
        || event->type() == QEvent::MacSizeChange
#endif
    )
        d->setLayoutItemMargins(QStyle::SE_SpinBoxLayoutItem);
    return QAbstractSpinBox::event(event);
}

QAbstractSpinBox::StepEnabled QCtmLongLongSpinBox::stepEnabled() const
{
    Q_D(const QCtmLongLongSpinBox);
    if (d->readOnly || d->type == QMetaType::UnknownType)
        return StepNone;
    if (d->wrapping)
        return StepEnabled(StepUpEnabled | StepDownEnabled);
    StepEnabled ret = StepNone;
    if (QAbstractSpinBoxPrivate::variantCompare(d->value, d->maximum) < 0)
    {
        ret |= StepUpEnabled;
    }
    if (QAbstractSpinBoxPrivate::variantCompare(d->value, d->minimum) > 0)
    {
        ret |= StepDownEnabled;
    }
    return ret;
}

void QCtmLongLongSpinBox::stepBy(int steps)
{
    Q_D(QCtmLongLongSpinBox);

    const QVariant old = d->value;
    QString tmp        = d->edit->displayText();
    int cursorPos      = d->edit->cursorPosition();
    bool dontstep      = false;
    EmitPolicy e       = EmitIfChanged;
    if (d->pendingEmit)
    {
        dontstep   = validate(tmp, cursorPos) != QValidator::Acceptable;
        d->cleared = false;
        d->interpret(NeverEmit);
        if (d->value != old)
            e = AlwaysEmit;
    }
    if (!dontstep)
    {
        QVariant singleStep;
        switch (d->stepType)
        {
        case QAbstractSpinBox::StepType::AdaptiveDecimalStepType:
            singleStep = d->calculateAdaptiveDecimalStep(steps);
            break;
        default:
            singleStep = d->singleStep;
        }
        d->setValue(d->bound(d->value.toLongLong() + (singleStep.toLongLong() * steps), old, steps), e);
    }
    else if (e == AlwaysEmit)
    {
        d->emitSignals(e, old);
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (style()->styleHint(QStyle::SH_SpinBox_SelectOnStep, nullptr, this, nullptr))
#endif
        selectAll();
}