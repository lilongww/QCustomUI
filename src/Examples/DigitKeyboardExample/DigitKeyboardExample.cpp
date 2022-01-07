#include "DigitKeyboardExample.h"

#include <QCustomUi/QCtmDigitKeyboard.h>

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>

DigitKeyboardExample::DigitKeyboardExample(QWidget* parent)
    : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    auto spinbox = new QSpinBox(this);
    spinbox->setSuffix("KHz");
    layout->addWidget(spinbox);
    auto doubleSpinBox = new QDoubleSpinBox(this);
    layout->addWidget(doubleSpinBox);
    doubleSpinBox->setSuffix("MHz");

    auto keyboard1 = new QCtmDigitKeyboard(this);
    keyboard1->bindBox(spinbox);
    keyboard1->setUnits({ "Hz", "KHz", "MHz", "GHz" });
    auto keyboard2 = new QCtmDigitKeyboard(this);
    keyboard2->bindBox(doubleSpinBox);
    keyboard2->setUnits({ "Hz", "KHz", "MHz", "GHz", "THz" });
}
