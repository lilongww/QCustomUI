#include "MessageBoxExample.h"

#include <QCustomUi/QCtmMessageBox.h>

MessageBoxExample::MessageBoxExample(QWidget* parent) : QCtmWindow(parent)
{
    ui.setupUi(this->centralWidget());
    init();
}

MessageBoxExample::~MessageBoxExample() {}

void MessageBoxExample::init()
{
    connect(
        ui.infoBtn, &QPushButton::clicked, this, [=] { QCtmMessageBox::information(this, tr("Information"), tr("Information content")); });
    connect(ui.warningBtn, &QPushButton::clicked, this, [=] { QCtmMessageBox::warning(this, tr("Warning"), tr("Warning content")); });
    connect(ui.criticalBtn, &QPushButton::clicked, this, [=] { QCtmMessageBox::critical(this, tr("Critical"), tr("Critical content")); });
    connect(ui.questionBtn, &QPushButton::clicked, this, [=] { QCtmMessageBox::question(this, tr("Question"), tr("Question content")); });
}
