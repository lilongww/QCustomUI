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

#include "QCtmDigitKeyboard.h"
#include "ui_QCtmDigitKeyboard.h"

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QKeyEvent>

struct QCtmDigitKeyboard::Impl
{
    InputMode mode{ InputMode::IntInput };
    QVariant value;
    QVariant maximum;
    QVariant minimum;
    QVariant step;
    std::vector<QString> units;
    int currentUnitIndex{ 0 };
    Ui::QCtmDigitKeyboard ui;
    QAbstractSpinBox* box{ nullptr };
    int decimals{ 2 };
    QAbstractSpinBox* bindedBox{ nullptr };
};

/*!
    \class      QCtmDigitKeyboard
    \brief      数字输入键盘.
    \inherits   QCtmDialog
    \ingroup    qcustomui
    \inmodule   qcustomui
    \inheaderfile QCtmDigitKeyboard.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmDigitKeyboard::QCtmDigitKeyboard(QWidget* parent /*= nullptr*/)
    : QCtmDialog(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->ui.setupUi(centralWidget());
    init();
}

/*!
    \brief      析构函数.
*/
QCtmDigitKeyboard::~QCtmDigitKeyboard()
{

}

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
QCtmDigitKeyboard::InputMode QCtmDigitKeyboard::inputMode() const
{
    return m_impl->mode;
}

/*!
    \brief      设置输入窗口中的值 \a value.
    \sa         value
*/
void QCtmDigitKeyboard::setValue(const QVariant& value)
{
    if (value != m_impl->value)
    {
        m_impl->value = value;
        if (m_impl->box)
            m_impl->box->setProperty("value", value);
    }
}

/*!
    \brief      返回输入的值.
    \sa         setValue
*/
QVariant QCtmDigitKeyboard::value() const
{
    return m_impl->box ? m_impl->box->property("value") : m_impl->value;
}

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
QVariant QCtmDigitKeyboard::maximum() const
{
    return m_impl->box ? m_impl->box->property("maximum") : m_impl->maximum;
}

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
QVariant QCtmDigitKeyboard::minimum() const
{
    return m_impl->box ? m_impl->box->property("minimum") : m_impl->minimum;
}

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
QVariant QCtmDigitKeyboard::singleStep() const
{
    return m_impl->box ? m_impl->box->property("singleStep") : m_impl->step;
}

/*!
    \brief      设置可选单位列表 \a units.
    \sa         units
*/
void QCtmDigitKeyboard::setUnits(const std::vector<QString>& units)
{
    m_impl->units = units;
    clearUnits();
    createUnits();
    if (m_impl->box)
    {
        m_impl->currentUnitIndex = 0;
        m_impl->box->setProperty("suffix", units.empty() ? "" : units[m_impl->currentUnitIndex]);
    }
}

/*!
    \brief      返回可选单位列表.
    \sa         setUnits
*/
const std::vector<QString>& QCtmDigitKeyboard::units() const
{
    return m_impl->units;
}

/*!
    \brief      设置当前选中的单位序号 \a index.
    \sa         currentUnitIndex
*/
void QCtmDigitKeyboard::setCurrentUnitIndex(int index)
{
    m_impl->currentUnitIndex = index;
    if (m_impl->box)
    {
        m_impl->box->setProperty("suffix", m_impl->units.empty() ? "" : m_impl->units[m_impl->currentUnitIndex]);
    }
}

/*!
    \brief      返回当前选中的单位序号.
    \sa         setCurrentUnitIndex
*/
int QCtmDigitKeyboard::currentUnitIndex() const
{
    return m_impl->currentUnitIndex;
}

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
int QCtmDigitKeyboard::decimals() const
{
    return m_impl->decimals;
}

/*!
    \brief      绑定到 QSpinBox/QDoubleSpinBox \a box.
*/
void QCtmDigitKeyboard::bindBox(QAbstractSpinBox* box)
{
    setInputMode(qobject_cast<QSpinBox*>(box) ? InputMode::IntInput : InputMode::DoubleInput);
    if (m_impl->bindedBox)
        m_impl->bindedBox->findChild<QLineEdit*>()->removeEventFilter(this);
    m_impl->bindedBox = box;
    connect(box, &QObject::destroyed, this, [=]() {m_impl->bindedBox = nullptr; });
    box->findChild<QLineEdit*>()->installEventFilter(this);
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
    if (m_impl->bindedBox && obj == m_impl->bindedBox->findChild<QLineEdit*>())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            auto evt = dynamic_cast<QMouseEvent*>(event);
            if (evt && evt->button() == Qt::LeftButton)
            {
                if (exec() == QDialog::Accepted)
                {
                    m_impl->bindedBox->setProperty("value", value());
                    m_impl->bindedBox->setProperty("suffix", m_impl->box->property("suffix"));
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
            box->setSuffix(m_impl->units.at(m_impl->currentUnitIndex));
        m_impl->box = box;
        setValue(box->value());
        connect(box, &QSpinBox::valueChanged, this, [this](auto val)
            {
                emit valueChanged(val);
            });
    }
    else
    {
        auto box = new QDoubleSpinBox(this);
        box->setSingleStep(m_impl->step.toDouble());
        box->setRange(m_impl->minimum.toDouble(), m_impl->maximum.toDouble());
        box->setDecimals(m_impl->decimals);
        box->setValue(m_impl->value.toDouble());
        if (!m_impl->units.empty())
            box->setSuffix(m_impl->units.at(m_impl->currentUnitIndex));
        m_impl->box = box;
        setValue(box->value());
        connect(box, &QDoubleSpinBox::valueChanged, this, [this](auto val)
            {
                emit valueChanged(val);
            });
    }
    m_impl->ui.inputLayout->addWidget(m_impl->box);

    auto sendBox = [=](Qt::Key key)
    {
        {
            auto evt = new QKeyEvent(QEvent::KeyPress, key, Qt::KeyboardModifier::NoModifier, key < Qt::Key_AsciiTilde ? QChar(key) : QString());
            qApp->postEvent(m_impl->box->findChild<QLineEdit*>(), evt);
        }
        {
            auto evt = new QKeyEvent(QEvent::KeyRelease, key, Qt::KeyboardModifier::NoModifier, key < Qt::Key_AsciiTilde ? QChar(key) : QString());
            qApp->postEvent(m_impl->box->findChild<QLineEdit*>(), evt);
        }
    };

    connect(m_impl->ui.keyBackspace, &QPushButton::clicked, m_impl->box, std::bind(sendBox, Qt::Key_Backspace));
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            char number = row * 3 + col + 1;
            auto btn = qobject_cast<QPushButton*>(m_impl->ui.digitLayout->itemAtPosition(row, col)->widget());
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

void QCtmDigitKeyboard::ensureConnect() const
{
    const_cast<QCtmDigitKeyboard*>(this)->ensureConnect();
}

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
    connect(m_impl->ui.buttonBox, &QDialogButtonBox::accepted, this, &QCtmDialog::accept);
    connect(m_impl->ui.buttonBox, &QDialogButtonBox::rejected, this, &QCtmDialog::reject);
}

void QCtmDigitKeyboard::clearUnits()
{
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
        auto btn = new QPushButton(m_impl->units[i], this);
        m_impl->ui.unitsLayout->addWidget(btn, i % 4, i / 4, Qt::AlignTop);
        connect(btn, &QPushButton::clicked, this, [=]()
            {
                setCurrentUnitIndex(i);
                accept();
            });
    }
    if (!m_impl->units.empty() && m_impl->units.size() < 4)
    {
        for (int i = m_impl->units.size(); i < 4; i++)
        {
            m_impl->ui.unitsLayout->addItem(new QSpacerItem(0, 0), i % 4, i / 4, Qt::AlignTop);
        }
    }
}

void QCtmDigitKeyboard::syncBindBox()
{
    auto pv = [=](const char* str)
    {
        return m_impl->bindedBox->property(str);
    };

    if (m_impl->bindedBox)
    {
        setRange(pv("minimum"), pv("maximum"));
        setValue(pv("value"));
        if (m_impl->mode == InputMode::DoubleInput)
            setDecimals(pv("decimals").toInt());
        setSingleStep(pv("singleStep"));
        if (m_impl->units.empty())
        {
            m_impl->bindedBox->setProperty("suffix", pv("suffix"));
        }
        else
        {
            auto unit = pv("suffix").toString();
            auto it = std::find(m_impl->units.begin(), m_impl->units.end(), unit);
            if (it != m_impl->units.end())
            {
                setCurrentUnitIndex(static_cast<int>(std::distance(m_impl->units.begin(), it)));
            }
        }
    }
}
