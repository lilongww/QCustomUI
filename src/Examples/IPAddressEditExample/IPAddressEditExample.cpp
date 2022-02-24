#include "IPAddressEditExample.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QHBoxLayout>

IPAddressEditExample::IPAddressEditExample(QWidget* parent) : QCtmWindow(parent) { init(); }

IPAddressEditExample::~IPAddressEditExample() {}

void IPAddressEditExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    auto edit = new QCtmIPAddressEdit(this);
    edit->setIPAddress("192.168.1.1");
    layout->addWidget(edit);
    layout->addStretch(1);
}
