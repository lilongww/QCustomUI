#include "HexLineEditExample.h"

#include <QCustomUi/QCtmHexLineEdit.h>
#include <QCustomUi/QCtmMessageBox.h>

#include <QDebug>
#include <QHBoxLayout>

HexLineEditExample::HexLineEditExample(QWidget* parent) : QCtmWindow(parent) { init(); }

HexLineEditExample::~HexLineEditExample() {}

void HexLineEditExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addWidget(new QCtmHexLineEdit(this));
}
