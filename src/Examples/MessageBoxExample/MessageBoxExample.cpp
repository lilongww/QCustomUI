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
    connect(ui.infoBtn_2,
            &QPushButton::clicked,
            this,
            [=] { QCtmMessageBox::information(this, tr("Information"), tr("Information content"), std::chrono::seconds { 3 }); });
    connect(ui.warningBtn_2,
            &QPushButton::clicked,
            this,
            [=] { QCtmMessageBox::warning(this, tr("Warning"), tr("Warning content"), std::chrono::seconds { 5 }); });
    connect(ui.criticalBtn_2,
            &QPushButton::clicked,
            this,
            [=] { QCtmMessageBox::critical(this, tr("Critical"), tr("Critical content"), std::chrono::seconds { 10 }); });
    connect(ui.questionBtn_2,
            &QPushButton::clicked,
            this,
            [=] { QCtmMessageBox::question(this, tr("Question"), tr("Question content"), std::chrono::seconds { 20 }); });
}
