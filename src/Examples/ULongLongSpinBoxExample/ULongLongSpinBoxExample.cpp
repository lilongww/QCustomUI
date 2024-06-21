#include "ULongLongSpinBoxExample.h"

#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmULongLongSpinBox.h>

#include <QDebug>
#include <QHBoxLayout>

ULongLongSpinBoxExample::ULongLongSpinBoxExample(QWidget* parent) : QCtmWindow(parent) { init(); }

ULongLongSpinBoxExample::~ULongLongSpinBoxExample() {}

void ULongLongSpinBoxExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    auto box = new QCtmULongLongSpinBox(this);
    box->setRange(std::numeric_limits<qulonglong>::min(), std::numeric_limits<qulonglong>::max());
    connect(box,
            &QCtmULongLongSpinBox::valueChanged,
            this,
            [this](qulonglong value)
            {
                qDebug() << QString::number(value); // QCtmMessageBox::information(this, "Value Changed", QString::number(value));
            });
    box->setSuffix("V");
    box->setPrefix("Data:");
    layout->addWidget(box);
    layout->addStretch(1);
}
