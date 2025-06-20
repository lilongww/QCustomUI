#include "IPAddressEditExample.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

IPAddressEditExample::IPAddressEditExample(QWidget* parent) : QCtmWindow(parent)
{
    init();
}

IPAddressEditExample::~IPAddressEditExample()
{
}

void IPAddressEditExample::init()
{
    auto layout = new QGridLayout(this->centralWidget());
    layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed), 0, 0);
    int row      = 0;
    auto addEdit = [&]()
    {
        auto edit = new QCtmIPAddressEdit(this);
        edit->setIPAddress("192.168.100.100");
        layout->addWidget(edit, row, 1);
        auto btn = new QPushButton("clear", this);
        connect(btn, &QPushButton::clicked, edit, &QCtmIPAddressEdit::clear);
        layout->addWidget(btn, row++, 2);
        return edit;
    };
    addEdit()->setAlignment(Qt::AlignLeft);
    addEdit()->setAlignment(Qt::AlignRight);
    addEdit()->setAlignment(Qt::AlignCenter);
    layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Fixed), 0, 3);
}
