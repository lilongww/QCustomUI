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

#include "QCtmDigitKeyboard.h"
#include "QCtmMessageBox.h"
#include "ui_QCtmDigitKeyboard.h"

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QSpinBox>

#define QTBUG_107745

class QCtmDigitKeyboardInputHelper : public QObject
{
public:
    QCtmDigitKeyboardInputHelper(QAbstractSpinBox* box, const QCtmDigitKeyboard::Units& units, const QVariant& step)
        : QObject(box), m_bindedBox(box), m_units(units), m_step(step)
    {
    }

protected:
    bool eventFilter(QObject* watched, QEvent* event) override
    {
        if (m_bindedBox && m_bindedBox->isEnabled() && watched == m_bindedBox->findChild<QLineEdit*>())
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                if (!m_keyboard)
                {
                    m_keyboard = new QCtmDigitKeyboard(m_bindedBox);
                    m_keyboard->setUnits(m_units);
                    m_keyboard->setSingleStep(m_step);
                    m_keyboard->bindBox(m_bindedBox);
                    auto evt = dynamic_cast<QMouseEvent*>(event);
                    if (evt && evt->button() == Qt::LeftButton)
                    {
                        auto [beforeValue, beforeUnit] = m_keyboard->showBefore();
                        if (m_keyboard->exec() == QDialog::Accepted)
                        {
                            m_keyboard->showAfter(beforeValue, beforeUnit);
                        }
                    }
                    watched->removeEventFilter(this);
                    this->deleteLater();
                }
            }
        }
        return false;
    }

private:
    QAbstractSpinBox* m_bindedBox;
    QCtmDigitKeyboard::Units m_units;
    QVariant m_step;
    QCtmDigitKeyboard* m_keyboard { nullptr };
};

struct QCtmDigitKeyboard::Impl
{
    InputMode mode { InputMode::IntInput };
    QVariant value;
    QVariant maximum;
    QVariant minimum;
    QVariant step;
    Units units;
    int currentUnitIndex { 0 };
    Ui::QCtmDigitKeyboard ui;
    QAbstractSpinBox* box { nullptr };
    int decimals { 2 };
    QAbstractSpinBox* bindedBox { nullptr };
};

/*!
    \class      QCtmDigitKeyboard
    \brief      数字输入键盘.
    \inherits   QCtmDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDigitKeyboard.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmDigitKeyboard::QCtmDigitKeyboard(QWidget* parent /*= nullptr*/) : QCtmDialog(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->ui.setupUi(centralWidget());
    init();
}

/*!
    \brief      析构函数.
*/
QCtmDigitKeyboard::~QCtmDigitKeyboard() {}

/*!
    \brief      设置输入模式 \a mode.
    \sa         inputMode
    \note       仅在键盘窗口未显示之前设置有效，显示之后调用该方法将被忽略.
*/
void QCtmDigitKeyboard::setInputMode(InputMode mode)
{
    if (!m_impl->box)
        m_impl->mode = mode;
}

/*!
    \brief      返回输入模式.
    \sa         setInputMode
*/
QCtmDigitKeyboard::InputMode QCtmDigitKeyboard::inputMode() const { return m_impl->mode; }

/*!
    \brief      设置输入窗口中的值 \a value.
    \sa         value
*/
void QCtmDigitKeyboard::setValue(const QVariant& value)
{
    m_impl->value = value;
    if (m_impl->box)
    {
        if (auto sp = qobject_cast<QSpinBox*>(m_impl->box); sp)
        {
            sp->setValue(m_impl->value.toInt());
        }
        else
        {
            qobject_cast<QDoubleSpinBox*>(m_impl->box)->setValue(m_impl->value.toDouble());
        }
    }
}

/*!
    \brief      返回输入的值.
    \sa         setValue
*/
QVariant QCtmDigitKeyboard::value() const { return m_impl->box ? m_impl->box->property("value") : m_impl->value; }

/*!
    \brief      设置输入最大值 \a max.
    \sa         maximum
*/
void QCtmDigitKeyboard::setMaximum(const QVariant& max)
{
    m_impl->maximum = max;
    if (m_impl->box)
        m_impl->box->setProperty("maximum", max);
}

/*!
    \brief      返回输入最大值.
    \sa         setMaximum
*/
QVariant QCtmDigitKeyboard::maximum() const { return m_impl->box ? m_impl->box->property("maximum") : m_impl->maximum; }

/*!
    \brief      设置输入最小值 \a min.
    \sa         minimum
*/
void QCtmDigitKeyboard::setMinimum(const QVariant& min)
{
    m_impl->minimum = min;
    if (m_impl->box)
        m_impl->box->setProperty("minimum", min);
}

/*!
    \brief      返回输入最小值.
    \sa         setMinimum
*/
QVariant QCtmDigitKeyboard::minimum() const { return m_impl->box ? m_impl->box->property("minimum") : m_impl->minimum; }

/*!
    \brief      设置输入范围 \a min, \a max.
    \sa         setMaximum, setMinimum
*/
void QCtmDigitKeyboard::setRange(const QVariant& min, const QVariant& max)
{
    setMinimum(min);
    setMaximum(max);
}

/*!
    \brief      设置步进值 \a step.
    \sa         singleStep
*/
void QCtmDigitKeyboard::setSingleStep(const QVariant& step)
{
    m_impl->step = step;
    if (m_impl->box)
        m_impl->box->setProperty("singleStep", step);
}

/*!
    \brief      返回步进值.
    \sa         setSingleStep
*/
QVariant QCtmDigitKeyboard::singleStep() const { return m_impl->box ? m_impl->box->property("singleStep") : m_impl->step; }

/*!
    \brief      设置可选单位列表 \a units.
    \sa         units
*/
void QCtmDigitKeyboard::setUnits(const Units& units)
{
    m_impl->units = units;
    clearUnits();
    if (units.empty())
        return;
    createUnits();
    setCurrentUnitIndex(0);
}

/*!
    \brief      返回可选单位列表.
    \sa         setUnits
*/
const QCtmDigitKeyboard::Units& QCtmDigitKeyboard::units() const { return m_impl->units; }

/*!
    \brief      设置当前选中的单位序号 \a index.
    \sa         currentUnitIndex
*/
void QCtmDigitKeyboard::setCurrentUnitIndex(int index)
{
    m_impl->currentUnitIndex = index;
    if (m_impl->box && !m_impl->units.empty())
    {
        const auto& unit = m_impl->units[m_impl->currentUnitIndex];
        m_impl->box->setProperty("suffix", unit.unit);
    }
}

/*!
    \brief      返回当前选中的单位序号.
    \sa         setCurrentUnitIndex
*/
int QCtmDigitKeyboard::currentUnitIndex() const { return m_impl->currentUnitIndex; }

/*!
    \brief      设置小数点后保留位数 \a decimals.
    \sa         decimals
    \note       该方法仅 InputMode::DoubleInput 有效.
*/
void QCtmDigitKeyboard::setDecimals(int decimals)
{
    m_impl->decimals = decimals;
    if (m_impl->box && m_impl->mode == InputMode::DoubleInput)
    {
        m_impl->box->setProperty("decimals", decimals);
    }
}

/*!
    \brief      返回小数点后保留位数.
    \sa         setDecimals
*/
int QCtmDigitKeyboard::decimals() const { return m_impl->decimals; }

/*!
    \brief      绑定到 QSpinBox/QDoubleSpinBox \a box.
*/
void QCtmDigitKeyboard::bindBox(QAbstractSpinBox* box)
{
    setInputMode(qobject_cast<QSpinBox*>(box) ? InputMode::IntInput : InputMode::DoubleInput);
    if (m_impl->bindedBox)
        m_impl->bindedBox->findChild<QLineEdit*>()->removeEventFilter(this);
    m_impl->bindedBox = box;
    connect(box, &QObject::destroyed, this, [=]() { m_impl->bindedBox = nullptr; });
    if (m_impl->mode == InputMode::IntInput)
    {
        connect(qobject_cast<QSpinBox*>(box), qOverload<int>(&QSpinBox::valueChanged), this, &QCtmDigitKeyboard::setValue);
    }
    else
    {
        connect(qobject_cast<QDoubleSpinBox*>(box), qOverload<double>(&QDoubleSpinBox::valueChanged), this, &QCtmDigitKeyboard::setValue);
    }
    box->findChild<QLineEdit*>()->installEventFilter(this);
}

/*!
    \brief      打开 \a box 的数字键盘，单位列表为 \a units, 步进为 \a step, 输入完成后销毁数字键盘.
*/
void QCtmDigitKeyboard::simpleBindBox(QAbstractSpinBox* box, const Units& units /*= {}*/, const QVariant& step /*= {}*/)
{
    box->findChild<QLineEdit*>()->installEventFilter(new QCtmDigitKeyboardInputHelper(box, units, step));
}

/*!
    \reimp
*/
QSize QCtmDigitKeyboard::sizeHint() const
{
    ensureConnect();
    return QCtmDialog::sizeHint();
}

/*!
    \reimp
*/
QSize QCtmDigitKeyboard::minimumSizeHint() const
{
    ensureConnect();
    return QCtmDialog::minimumSizeHint();
}

/*!
    \reimp
*/
bool QCtmDigitKeyboard::eventFilter(QObject* obj, QEvent* event)
{
    if (m_impl->bindedBox && m_impl->bindedBox->isEnabled() && obj == m_impl->bindedBox->findChild<QLineEdit*>())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            auto evt = dynamic_cast<QMouseEvent*>(event);
            if (evt && evt->button() == Qt::LeftButton)
            {
                auto [beforeValue, beforeUnit] = showBefore();
                if (exec() == QDialog::Accepted)
                {
                    showAfter(beforeValue, beforeUnit);
                }
            }
        }
    }
    return QCtmDialog::eventFilter(obj, event);
}

void QCtmDigitKeyboard::showEvent(QShowEvent* event)
{
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    syncBindBox();
    if (m_impl->box)
    {
        m_impl->box->setFocus();
        m_impl->box->selectAll();
    }
    QDialog::showEvent(event);
}

void QCtmDigitKeyboard::ensureConnect()
{
    if (m_impl->box)
        return;
    if (m_impl->mode == InputMode::IntInput)
    {
        auto box = new QSpinBox(this);
        box->setSingleStep(m_impl->step.toInt());
        box->setRange(m_impl->minimum.toInt(), m_impl->maximum.toInt());
        box->setValue(m_impl->value.toDouble());
        if (!m_impl->units.empty())
        {
            box->setSuffix(m_impl->units.at(m_impl->currentUnitIndex).unit);
        }
        m_impl->box = box;
        setValue(box->value());
        connect(box, qOverload<int>(&QSpinBox::valueChanged), this, [this](auto val) { emit valueChanged(val); });
    }
    else
    {
        auto box = new QDoubleSpinBox(this);
        box->setSingleStep(m_impl->step.toDouble());
        box->setRange(m_impl->minimum.toDouble(), m_impl->maximum.toDouble());
        box->setDecimals(m_impl->decimals);
        box->setValue(m_impl->value.toDouble());
        if (!m_impl->units.empty())
        {
            box->setSuffix(m_impl->units.at(m_impl->currentUnitIndex).unit);
        }
        m_impl->box = box;
        setValue(box->value());
        connect(box, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](auto val) { emit valueChanged(val); });
    }
    m_impl->ui.inputLayout->addWidget(m_impl->box);

    auto sendBox = [=](Qt::Key key)
    {
        {
            auto evt =
                new QKeyEvent(QEvent::KeyPress, key, Qt::KeyboardModifier::NoModifier, key < Qt::Key_AsciiTilde ? QChar(key) : QString());
            qApp->postEvent(m_impl->box->findChild<QLineEdit*>(), evt);
        }
        {
            auto evt =
                new QKeyEvent(QEvent::KeyRelease, key, Qt::KeyboardModifier::NoModifier, key < Qt::Key_AsciiTilde ? QChar(key) : QString());
            qApp->postEvent(m_impl->box->findChild<QLineEdit*>(), evt);
        }
    };

    connect(m_impl->ui.keyBackspace, &QPushButton::clicked, m_impl->box, std::bind(sendBox, Qt::Key_Backspace));
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            char number = row * 3 + col + 1;
            auto btn    = qobject_cast<QPushButton*>(m_impl->ui.digitLayout->itemAtPosition(row, col)->widget());
            connect(btn, &QPushButton::clicked, m_impl->box, std::bind(sendBox, static_cast<Qt::Key>(Qt::Key_0 + number)));
        }
    }

    auto btn = qobject_cast<QPushButton*>(m_impl->ui.digitLayout->itemAtPosition(3, 1)->widget());
    connect(btn, &QPushButton::clicked, m_impl->box, std::bind(sendBox, Qt::Key_0));
    btn = qobject_cast<QPushButton*>(m_impl->ui.digitLayout->itemAtPosition(3, 2)->widget());
    connect(btn, &QPushButton::clicked, m_impl->box, std::bind(sendBox, Qt::Key_Period));
    btn->setVisible(m_impl->mode == InputMode::DoubleInput);
    syncBindBox();
}

void QCtmDigitKeyboard::ensureConnect() const { const_cast<QCtmDigitKeyboard*>(this)->ensureConnect(); }

void QCtmDigitKeyboard::init()
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            auto btn = new QPushButton(QString::number(row * 3 + col + 1), this);
            btn->setFocusPolicy(Qt::NoFocus);
            m_impl->ui.digitLayout->addWidget(btn, row, col);
        }
    }
    auto btn = new QPushButton("0", this);
    btn->setFocusPolicy(Qt::NoFocus);
    m_impl->ui.digitLayout->addWidget(btn, 3, 1);
    btn = new QPushButton(".", this);
    btn->setFocusPolicy(Qt::NoFocus);
    m_impl->ui.digitLayout->addWidget(btn, 3, 2);
    connect(m_impl->ui.buttonBox,
            &QDialogButtonBox::accepted,
            this,
            [=]()
            {
                if (!m_impl->units.empty())
                    acceptUnit(m_impl->currentUnitIndex);
                else
                    accept();
            });
    connect(m_impl->ui.buttonBox, &QDialogButtonBox::rejected, this, &QCtmDialog::reject);
}

void QCtmDigitKeyboard::clearUnits()
{
    if (m_impl->box)
    {
        m_impl->box->setProperty("suffix", "");
    }
    for (int row = m_impl->ui.unitsLayout->rowCount() - 1; row >= 0; row--)
    {
        for (int col = m_impl->ui.unitsLayout->columnCount() - 1; col >= 0; col--)
        {
            auto item = m_impl->ui.unitsLayout->itemAtPosition(row, col);
            if (item && item->widget())
                delete item->widget();
            else
                delete item;
        }
    }
}

void QCtmDigitKeyboard::createUnits()
{
    for (int i = 0; i < m_impl->units.size(); i++)
    {
        auto btn = new QPushButton(m_impl->units[i].unit, this);
        m_impl->ui.unitsLayout->addWidget(btn, i % 4, i / 4, Qt::AlignTop);
        connect(btn, &QPushButton::clicked, this, std::bind(&QCtmDigitKeyboard::acceptUnit, this, i));
    }

    if (!m_impl->units.empty() && m_impl->units.size() < 4)
    {
        for (int i = static_cast<int>(m_impl->units.size()); i < 4; i++)
        {
            m_impl->ui.unitsLayout->addItem(new QSpacerItem(0, 0), i % 4, i / 4, Qt::AlignTop);
        }
    }
}

void QCtmDigitKeyboard::syncBindBox()
{
    auto pv = [=](const char* str) { return m_impl->bindedBox->property(str); };

    if (m_impl->bindedBox)
    {
        if (m_impl->units.empty())
            setRange(pv("minimum"), pv("maximum"));
        setValue(pv("value"));
        if (m_impl->mode == InputMode::DoubleInput)
            setDecimals(pv("decimals").toInt());
        setSingleStep(pv("singleStep"));
        if (m_impl->units.empty())
        {
            m_impl->box->setProperty("suffix", pv("suffix"));
        }
        else
        {
            auto unit = pv("suffix").toString();
            auto it   = std::find_if(m_impl->units.begin(), m_impl->units.end(), [&](const auto& u) { return u.unit == unit; });
            if (it != m_impl->units.end())
            {
                setCurrentUnitIndex(static_cast<int>(std::distance(m_impl->units.begin(), it)));
            }
        }
    }

    if (m_impl->box && !m_impl->units.empty())
    {
        auto minIt = std::min_element(m_impl->units.begin(),
                                      m_impl->units.end(),
                                      [=](const auto& u1, const auto& u2)
                                      {
                                          if (m_impl->mode == InputMode::IntInput)
                                              return u1.minimum.toInt() < u2.minimum.toInt();
                                          else
                                              return u1.minimum.toDouble() < u2.minimum.toDouble();
                                      });
        auto maxIt = std::max_element(m_impl->units.begin(),
                                      m_impl->units.end(),
                                      [=](const auto& u1, const auto& u2)
                                      {
                                          if (m_impl->mode == InputMode::IntInput)
                                              return u1.maximum.toInt() < u2.maximum.toInt();
                                          else
                                              return u1.maximum.toDouble() < u2.maximum.toDouble();
                                      });
        m_impl->box->setProperty("minimum", minIt->minimum);
        m_impl->box->setProperty("maximum", maxIt->maximum);
    }
}

void QCtmDigitKeyboard::acceptUnit(int unitIndex)
{
    auto value = m_impl->box->property("value");
    setCurrentUnitIndex(unitIndex);
    const auto& unit = m_impl->units.at(unitIndex);
    QString error;
    auto valueCheck = [this, &unit, &error](const QVariant& value)
    {
        if (m_impl->mode == InputMode::IntInput)
        {
            auto v   = value.toInt();
            auto max = unit.maximum.toInt();
            auto min = unit.minimum.toInt();
            error    = tr("%1%2 - %3%4").arg(min).arg(unit.unit).arg(max).arg(unit.unit);
            return min <= v && v <= max;
        }
        else
        {
            auto v   = value.toDouble();
            auto max = unit.maximum.toDouble();
            auto min = unit.minimum.toDouble();
            error    = tr("%1%2 - %3%4").arg(min).arg(unit.unit).arg(max).arg(unit.unit);
            return min <= v && v <= max;
        }
    };
    if (!valueCheck(value))
    {
        QCtmMessageBox::warning(this, tr("Warning"), tr("Value out of range: %1").arg(error));
        m_impl->box->setFocus();
        m_impl->box->selectAll();
        return;
    }
    accept();
}

std::pair<QVariant, QString> QCtmDigitKeyboard::showBefore()
{
    auto beforeValue = m_impl->bindedBox->property("value");
    auto beforeUnit  = m_impl->bindedBox->property("suffix").toString();
    setValue(beforeValue);
    return std::make_pair(beforeValue, beforeUnit);
}

void QCtmDigitKeyboard::showAfter(const QVariant& beforeValue, const QString& beforeUnit)
{
    auto val  = value();
    auto unit = m_impl->box->property("suffix").toString();
    m_impl->bindedBox->blockSignals(true);
    if (!m_impl->units.empty())
    {
        m_impl->bindedBox->setProperty("minimum", m_impl->units[m_impl->currentUnitIndex].minimum);
        m_impl->bindedBox->setProperty("maximum", m_impl->units[m_impl->currentUnitIndex].maximum);
    }
    m_impl->bindedBox->setProperty("suffix", unit);
    m_impl->bindedBox->setProperty("value", val);
    m_impl->bindedBox->blockSignals(false);
    if (beforeValue != val)
    {
        if (qobject_cast<QDoubleSpinBox*>(m_impl->bindedBox))
            QMetaObject::invokeMethod(m_impl->bindedBox, "valueChanged", Q_ARG(double, val.toDouble()));
        else if (qobject_cast<QSpinBox*>(m_impl->bindedBox))
            QMetaObject::invokeMethod(m_impl->bindedBox, "valueChanged", Q_ARG(int, val.toInt()));
        QMetaObject::invokeMethod(m_impl->bindedBox, "textChanged", Q_ARG(QString, m_impl->bindedBox->text()));
    }
    else if (beforeUnit != unit)
        QMetaObject::invokeMethod(m_impl->bindedBox, "textChanged", Q_ARG(QString, m_impl->bindedBox->text()));
}
