#include "SerialPortComboBoxExample.h"

#include <QCustomUi/QCtmSerialPortComboBox.h>

#include <QDebug>
#include <QHBoxLayout>

SerialPortComboBoxExample::SerialPortComboBoxExample(QWidget* parent) : QCtmWindow(parent) { init(); }

SerialPortComboBoxExample::~SerialPortComboBoxExample() {}

void SerialPortComboBoxExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    auto box = new QCtmSerialPortComboBox(this);
    layout->addWidget(box);
    layout->addStretch(1);
}
