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
#include "QCtmULongLongSpinBox.h"

#include <QStyle>
#include <QStyleOption>
#include <private/qabstractspinbox_p.h>

/*!
    \class      QCtmULongLongSpinBox
    \brief      64位无符号长整型输入框，弥补Qt库QSpinBox输入框仅支持32位整型，API接口与QSpinBox一致.
    \inherits   QAbstractSpinBox
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmULongLongSpinBox.h
*/

class QCtmULongLongSpinBoxPrivate : public QAbstractSpinBoxPrivate
{
    Q_DECLARE_PUBLIC(QCtmULongLongSpinBox)
public:
    QCtmULongLongSpinBoxPrivate();
    void emitSignals(EmitPolicy ep, const QVariant&) override;
    virtual QVariant valueFromText(const QString& n) const override;
    virtual QString textFromValue(const QVariant& n) const override;
    QVariant validateAndInterpret(QString& input, int& pos, QValidator::State& state) const;
    inline void init()
    {
        Q_Q(QCtmULongLongSpinBox);
        q->setInputMethodHints(Qt::ImhDigitsOnly);
        setLayoutItemMargins(QStyle::SE_SpinBoxLayoutItem);
    }
    QVariant calculateAdaptiveDecimalStep(int steps) const override;

    int displayIntegerBase;
    int displayFieldWidth { 0 };
    QChar displayFillChar { u8' ' };
};

QCtmULongLongSpinBoxPrivate::QCtmULongLongSpinBoxPrivate()
{
    minimum            = 0ULL;
    maximum            = 99ULL;
    value              = minimum;
    displayIntegerBase = 10;
    singleStep         = 1ULL;
    type               = QMetaType::ULongLong;
}

void QCtmULongLongSpinBoxPrivate::emitSignals(EmitPolicy ep, const QVariant& old)
{
    Q_Q(QCtmULongLongSpinBox);
    if (ep != NeverEmit)
    {
        pendingEmit = false;
        if (ep == AlwaysEmit || value != old)
        {
            emit q->textChanged(edit->displayText());
            emit q->valueChanged(value.toULongLong());
        }
    }
}

QVariant QCtmULongLongSpinBoxPrivate::valueFromText(const QString& text) const
{
    Q_Q(const QCtmULongLongSpinBox);

    return QVariant(q->valueFromText(text));
}

QString QCtmULongLongSpinBoxPrivate::textFromValue(const QVariant& n) const
{
    Q_Q(const QCtmULongLongSpinBox);
    return q->textFromValue(n.toULongLong());
}

QVariant QCtmULongLongSpinBoxPrivate::validateAndInterpret(QString& input, int& pos, QValidator::State& state) const
{
    if (cachedText == input && !input.isEmpty())
    {
        state = cachedState;
        // QSBDEBUG() << "cachedText was '" << cachedText << "' state was " << state << " and value was " << cachedValue;

        return cachedValue;
    }
    const auto max = maximum.toULongLong();
    const auto min = minimum.toULongLong();

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
            num = copy.toULongLong(&ok, displayIntegerBase);
        }
        else
        {
            num = locale.toULongLong(copy, &ok);
            if (!ok && (max >= 1000 || min <= -1000))
            {
                const QString sep(locale.groupSeparator());
                const QString doubleSep = sep + sep;
                if (copy.contains(sep) && !copy.contains(doubleSep))
                {
                    QString copy2 = copy;
                    copy2.remove(sep);
                    num = locale.toULongLong(copy2, &ok);
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

QVariant QCtmULongLongSpinBoxPrivate::calculateAdaptiveDecimalStep(int steps) const
{
    const auto intValue = value.toULongLong();
    const auto absValue = qAbs(intValue);

    if (absValue < 100ULL)
        return 1ULL;

    const bool valueNegative    = intValue < 0;
    const bool stepsNegative    = steps < 0;
    const auto signCompensation = (valueNegative == stepsNegative) ? 0ULL : 1ULL;

    const auto log = static_cast<qulonglong>(std::log10(absValue - signCompensation)) - 1ULL;
    return static_cast<qulonglong>(std::pow(10, log));
}

/*!
    \brief      构造函数 \a parent.
*/
QCtmULongLongSpinBox::QCtmULongLongSpinBox(QWidget* parent /*= nullptr*/) : QAbstractSpinBox(*new QCtmULongLongSpinBoxPrivate, parent)
{
    Q_D(QCtmULongLongSpinBox);
    d->init();
}

/*!
    \brief      析构函数.
*/
QCtmULongLongSpinBox::~QCtmULongLongSpinBox() {}

/*!
    \brief      设置值 \a value.
    \sa         value
*/
void QCtmULongLongSpinBox::setValue(qulonglong value)
{
    Q_D(QCtmULongLongSpinBox);
    d->setValue(QVariant(value), EmitIfChanged);
}

/*!
    \brief      获取值.
    \sa         setValue
*/
qulonglong QCtmULongLongSpinBox::value() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->value.toULongLong();
}

/*!
    \brief      设置前缀 \a prefix.
    \sa         prefix
*/
void QCtmULongLongSpinBox::setPrefix(const QString& prefix)
{
    Q_D(QCtmULongLongSpinBox);

    d->prefix = prefix;
    d->updateEdit();

    d->cachedSizeHint        = QSize();
    d->cachedMinimumSizeHint = QSize(); // minimumSizeHint cares about the prefix
    updateGeometry();
}

/*!
    \brief      返回前缀.
    \sa         setPrefix
*/
QString QCtmULongLongSpinBox::prefix() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->prefix;
}

/*!
    \brief      设置后缀 \a suffix.
    \sa         prefix
*/
void QCtmULongLongSpinBox::setSuffix(const QString& suffix)
{
    Q_D(QCtmULongLongSpinBox);

    d->suffix = suffix;
    d->updateEdit();

    d->cachedSizeHint = QSize();
    updateGeometry();
}

/*!
    \brief      返回后缀.
    \sa         setSuffix
*/
QString QCtmULongLongSpinBox::suffix() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->suffix;
}

/*!
    \brief      返回值的文本, 不包括前缀，后缀，前导空格，结尾空格.
*/
QString QCtmULongLongSpinBox::cleanText() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->stripped(d->edit->displayText());
}

/*!
    \brief      设置步进 \a value.
    \sa         singleStep
*/
void QCtmULongLongSpinBox::setSingleStep(qulonglong value)
{
    Q_D(QCtmULongLongSpinBox);
    if (value >= 0LL)
    {
        d->singleStep = QVariant(value);
        d->updateEdit();
    }
}

/*!
    \brief      返回步进.
    \sa         setSingleStep
*/
qulonglong QCtmULongLongSpinBox::singleStep() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->singleStep.toULongLong();
}

/*!
    \brief      设置最小值 \a minimum.
    \sa         minimum
*/
void QCtmULongLongSpinBox::setMinimum(qulonglong minimum)
{
    Q_D(QCtmULongLongSpinBox);
    const QVariant m(minimum);
    d->setRange(m, (QCtmULongLongSpinBoxPrivate::variantCompare(d->maximum, m) > 0 ? d->maximum : m));
}

/*!
    \brief      返回最小值.
    \sa         setMinimum
*/
qulonglong QCtmULongLongSpinBox::minimum() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->minimum.toULongLong();
}

/*!
    \brief      设置最大值 \a maximum.
    \sa         maximum
*/
void QCtmULongLongSpinBox::setMaximum(qulonglong maximum)
{
    Q_D(QCtmULongLongSpinBox);
    const QVariant m(maximum);
    d->setRange((QCtmULongLongSpinBoxPrivate::variantCompare(d->minimum, m) < 0 ? d->minimum : m), m);
}

/*!
    \brief      返回最大值.
    \sa         setMaximum
*/
qulonglong QCtmULongLongSpinBox::maximum() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->maximum.toULongLong();
}

/*!
    \brief      设置值范围 \a minimum, \a maximum.
*/
void QCtmULongLongSpinBox::setRange(qulonglong minimum, qulonglong maximum)
{
    Q_D(QCtmULongLongSpinBox);
    d->setRange(QVariant(minimum), QVariant(maximum));
}

/*!
    \brief      设置步进类型 \a stepType.
    \sa         stepType
*/
void QCtmULongLongSpinBox::setStepType(QAbstractSpinBox::StepType stepType)
{
    Q_D(QCtmULongLongSpinBox);
    d->stepType = stepType;
}

/*!
    \brief      返回步进类型.
    \sa         setStepType
*/
QAbstractSpinBox::StepType QCtmULongLongSpinBox::stepType() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->stepType;
}

/*!
    \brief      设置显示进制 \a base.
    \sa         displayIntegerBase
*/
void QCtmULongLongSpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QCtmULongLongSpinBox);
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

/*!
    \brief      返回显示进制.
    \sa         setDisplayIntegerBase
*/
int QCtmULongLongSpinBox::displayIntegerBase() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->displayIntegerBase;
}

/*!
    \brief      设置文本显示宽度 \a width, 文本不足显示宽度时，填充 displayFillChar.
    \sa         displayFieldWidth
*/
void QCtmULongLongSpinBox::setDisplayFieldWidth(int width)
{
    Q_D(QCtmULongLongSpinBox);
    d->displayFieldWidth = width;
    update();
}

/*!
    \brief      返回文本显示宽度.
    \sa         setDisplayFieldWidth
*/
int QCtmULongLongSpinBox::displayFieldWidth() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->displayFieldWidth;
}

/*!
    \brief      设置文本显示填充字符 \a ch.
    \sa         displayFillChar
*/
void QCtmULongLongSpinBox::setDisplayFillChar(QChar ch)
{
    Q_D(QCtmULongLongSpinBox);
    d->displayFillChar = ch;
    update();
}

/*!
    \brief      返回文本填充字符.
    \sa         setDisplayFillChar
*/
QChar QCtmULongLongSpinBox::displayFillChar() const
{
    Q_D(const QCtmULongLongSpinBox);
    return d->displayFillChar;
}

/*!
    \brief      将值 \a value 转换为文本.
    \sa         valueFromText
*/
QString QCtmULongLongSpinBox::textFromValue(qulonglong value) const
{
    Q_D(const QCtmULongLongSpinBox);
    QString str;

    if (d->displayIntegerBase != 10)
    {
        const auto prefix = value < 0ULL ? "-" : "";
        str               = prefix + QString::number(qAbs(value), d->displayIntegerBase);
    }
    else
    {
        str = locale().toString(value);
        if (!d->showGroupSeparator && (qAbs(value) >= 1000ULL || value == std::numeric_limits<qulonglong>::lowest()))
        {
            str.remove(locale().groupSeparator());
        }
    }
    if (d->displayFieldWidth > 0)
        str = QString("%1").arg(str, d->displayFieldWidth, d->displayFillChar);
    return str;
}

/*!
    \brief      将文本 \a text 转换为值.
    \sa         textFromValue
*/
qulonglong QCtmULongLongSpinBox::valueFromText(const QString& text) const
{
    Q_D(const QCtmULongLongSpinBox);

    QString copy            = text;
    int pos                 = d->edit->cursorPosition();
    QValidator::State state = QValidator::Acceptable;
    return d->validateAndInterpret(copy, pos, state).toULongLong();
}

/*!
    \reimp
*/
QValidator::State QCtmULongLongSpinBox::validate(QString& input, int& pos) const
{
    Q_D(const QCtmULongLongSpinBox);

    QValidator::State state;
    d->validateAndInterpret(input, pos, state);
    return state;
}

/*!
    \reimp
*/
void QCtmULongLongSpinBox::fixup(QString& str) const
{
    if (!isGroupSeparatorShown())
        str.remove(locale().groupSeparator());
}

/*!
    \reimp
*/
bool QCtmULongLongSpinBox::event(QEvent* event)
{
    Q_D(QCtmULongLongSpinBox);
    if (event->type() == QEvent::StyleChange
#ifdef Q_OS_MAC
        || event->type() == QEvent::MacSizeChange
#endif
    )
        d->setLayoutItemMargins(QStyle::SE_SpinBoxLayoutItem);
    return QAbstractSpinBox::event(event);
}

/*!
    \reimp
*/
QAbstractSpinBox::StepEnabled QCtmULongLongSpinBox::stepEnabled() const
{
    Q_D(const QCtmULongLongSpinBox);
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

/*!
    \reimp
*/
void QCtmULongLongSpinBox::stepBy(int steps)
{
    Q_D(QCtmULongLongSpinBox);

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
        d->setValue(d->bound(d->value.toULongLong() + (singleStep.toULongLong() * steps), old, steps), e);
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