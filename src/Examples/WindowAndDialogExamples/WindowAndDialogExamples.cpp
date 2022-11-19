#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmDialog.h>
#include <QCustomUi/QCtmLoadingDialog.h>
#include <QCustomUi/QCtmTitleBar.h>
#include <QCustomUi/QCtmMessageBox.h>

#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidgetAction>

#include <thread>

WindowAndDialogExamples::WindowAndDialogExamples(QWidget* parent) : QCtmWindow(parent)
{
    ui.setupUi(this->centralWidget());
    auto* dlg = new QCtmDialog(this);
    dlg->setWindowFlag(Qt::WindowMinMaxButtonsHint);
    auto layout = new QHBoxLayout(dlg->centralWidget());
    auto btn    = new QPushButton(dlg);
    layout->addWidget(btn);
    connect(ui.popDialogBtn, &QPushButton::clicked, this, [=]() { dlg->exec(); });
    connect(btn, &QPushButton::clicked, this, [=]() { QCtmMessageBox::information(dlg, "Test", "Test"); });

    this->titleBar()->addAction(new QAction(tr("Test Button")));
    auto action = new QWidgetAction(this);
    action->setDefaultWidget(new QLineEdit(this));
    this->titleBar()->addAction(action);
    auto menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    menuBar->addMenu(tr("&File"))->addAction(tr("&Open"));
    this->setStatusBar(new QStatusBar(this));
}
