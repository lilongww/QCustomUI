#include "TestDialog.h"

#include <QCustomUi/QCtmMessageBox.h>

TestDialog::TestDialog(QWidget* parent) : QCtmDialog(parent)
{
    ui.setupUi(centralWidget());
    connect(ui.okBtn, &QToolButton::clicked, this, &TestDialog::accept);
    connect(ui.dlgBtn,
            &QToolButton::clicked,
            this,
            [=]()
            {
                QCtmMessageBox::information(this, QStringLiteral("test"), QStringLiteral("test"));
                return;
            });
}

TestDialog::~TestDialog() {}
