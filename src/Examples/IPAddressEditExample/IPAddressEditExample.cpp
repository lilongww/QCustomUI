#include "IPAddressEditExample.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QPushButton>
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
    auto btn = new QPushButton("clear", this);
    connect(btn, &QPushButton::clicked, edit, &QCtmIPAddressEdit::clear);
    layout->addWidget(btn);
    layout->addStretch(1);
}
