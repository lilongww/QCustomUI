#include "InputExamples.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

#include <QHBoxLayout>

InputExamples::InputExamples(QWidget *parent)
    : QCtmWindow(parent)
{
    auto layout = new QHBoxLayout(this->centralWidget());
    QCtmIPAddressEdit* edit = new QCtmIPAddressEdit(this);
    edit->setIPAddress("127.0.0.1");
    layout->addStretch(1);
    layout->addWidget(edit);
    layout->addStretch(1);
}
