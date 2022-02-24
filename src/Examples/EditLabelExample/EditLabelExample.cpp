#include "EditLabelExample.h"

#include <QCustomUi/QCtmEditLabel.h>

#include <QHBoxLayout>

EditLabelExample::EditLabelExample(QWidget* parent) : QCtmWindow(parent) { init(); }

EditLabelExample::~EditLabelExample() {}

void EditLabelExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    layout->addWidget(new QCtmEditLabel(tr("Example Text"), this));
    layout->addStretch(1);
}
