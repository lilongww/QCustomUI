#include "DigitKeyboardExample.h"

#include <QCustomUi/QCtmDigitKeyboard.h>

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QDebug>

#include <ratio>

DigitKeyboardExample::DigitKeyboardExample(QWidget* parent) : QCtmWindow(parent)
{
    auto layout  = new QVBoxLayout(this->centralWidget());
    auto spinbox = new QSpinBox(this);
    spinbox->setSuffix("KHz");
    layout->addWidget(spinbox);
    auto doubleSpinBox = new QDoubleSpinBox(this);
    layout->addWidget(doubleSpinBox);
    doubleSpinBox->setSuffix("MHz");
    auto sp = new QSpinBox(this);
    sp->setSuffix("KHz");
    sp->setRange(0, 1000);
    sp->setValue(100);
    layout->addWidget(sp);

    auto keyboard1 = new QCtmDigitKeyboard(this);
    keyboard1->bindBox(spinbox);
    keyboard1->setUnits({ { "Hz", 10, std::giga::num }, { "KHz", 0, std::mega::num }, { "MHz", 0, std::kilo::num }, { "GHz", 0, 1 } });
    auto keyboard2 = new QCtmDigitKeyboard(this);
    keyboard2->bindBox(doubleSpinBox);
    keyboard2->setUnits({ { "Hz", 0, std::giga::num }, { "KHz", 0, std::mega::num }, { "MHz", 0, std::kilo::num }, { "GHz", 0, 1 } });
    auto keyboard3 = new QCtmDigitKeyboard(this);
    keyboard3->bindBox(sp);
    sp->setValue(10000);
    connect(spinbox, &QSpinBox::textChanged, this, [=](auto text) { qDebug() << text; });
}
