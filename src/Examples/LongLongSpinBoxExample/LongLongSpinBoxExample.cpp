#include "LongLongSpinBoxExample.h"

#include <QCustomUi/QCtmLongLongSpinBox.h>
#include <QCustomUi/QCtmMessageBox.h>

#include <QDebug>
#include <QHBoxLayout>

LongLongSpinBoxExample::LongLongSpinBoxExample(QWidget* parent) : QCtmWindow(parent) { init(); }

LongLongSpinBoxExample::~LongLongSpinBoxExample() {}

void LongLongSpinBoxExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    auto box = new QCtmLongLongSpinBox(this);
    box->setRange(std::numeric_limits<qlonglong>::min(), std::numeric_limits<qlonglong>::max());
    connect(box,
            &QCtmLongLongSpinBox::valueChanged,
            this,
            [this](qlonglong value)
            {
                qDebug() << QString::number(value); // QCtmMessageBox::information(this, "Value Changed", QString::number(value));
            });
    box->setSuffix("V");
    box->setPrefix("Data:");
    box->setDisplayFieldWidth(8);
    box->setDisplayFillChar('0');
    box->setDisplayIntegerBase(16);
    layout->addWidget(box);
    layout->addStretch(1);
}
