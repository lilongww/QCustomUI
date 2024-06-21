#include "DigitKeyboardExample.h"

#include <QCustomUi/QCtmDigitKeyboard.h>
#include <QCustomUi/QCtmLongLongSpinBox.h>
#include <QCustomUi/QCtmULongLongSpinBox.h>

#include <QDebug>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVBoxLayout>

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
    auto longlongSp = new QCtmLongLongSpinBox(this);
    longlongSp->setSuffix("KHz");
    longlongSp->setRange(0, 1000);
    longlongSp->setValue(100);
    layout->addWidget(longlongSp);

    auto ulonglongSp = new QCtmULongLongSpinBox(this);
    ulonglongSp->setSuffix("KHz");
    ulonglongSp->setRange(0, 1000);
    ulonglongSp->setValue(100);
    layout->addWidget(ulonglongSp);

    auto keyboard1 = new QCtmDigitKeyboard(this);
    keyboard1->bindBox(spinbox);
    keyboard1->setUnits({ { "Hz", -10, std::giga::num }, { "KHz", 0, std::mega::num }, { "MHz", 0, std::kilo::num }, { "GHz", 0, 1 } });
    auto keyboard2 = new QCtmDigitKeyboard(this);
    keyboard2->bindBox(doubleSpinBox);
    keyboard2->setUnits({ { "Hz", 0, std::giga::num }, { "KHz", 0, std::mega::num }, { "MHz", 0, std::kilo::num }, { "GHz", 0, 1 } });
    auto keyboard3 = new QCtmDigitKeyboard(this);
    keyboard3->bindBox(longlongSp);
    keyboard3->setUnits({ { "Hz", 0, std::exa::num },
                          { "KHz", 0, std::exa::num / 1000 },
                          { "MHz", 0, std::exa::num / 1000'000 },
                          { "GHz", 0, std::exa::num / 1000'000'000 } });
    longlongSp->setValue(10000);

    auto keyboard4 = new QCtmDigitKeyboard(this);
    keyboard4->bindBox(ulonglongSp);
    keyboard4->setUnits({ { "Hz", 0, std::exa::num },
                          { "KHz", 0, std::exa::num / 1000 },
                          { "MHz", 0, std::exa::num / 1000'000 },
                          { "GHz", 0, std::exa::num / 1000'000'000 } });
    ulonglongSp->setValue(10000);
    connect(spinbox, &QSpinBox::textChanged, this, [=](auto text) { qDebug() << text; });
    connect(doubleSpinBox, &QDoubleSpinBox::textChanged, this, [=](auto text) { qDebug() << text; });
    connect(longlongSp, &QCtmLongLongSpinBox::textChanged, this, [=](auto text) { qDebug() << text; });
    connect(ulonglongSp, &QCtmULongLongSpinBox::textChanged, this, [=](auto text) { qDebug() << text; });
}
