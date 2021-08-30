#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmTitleBar.h>
#include <QCustomUi/QCtmDialog.h>

#include <QWidgetAction>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>

WindowAndDialogExamples::WindowAndDialogExamples(QWidget* parent)
    : QCtmWindow(parent)
{
    ui.setupUi(this->centralWidget());
    auto* dlg = new QCtmDialog(this);
    dlg->setWindowFlag(Qt::WindowMinMaxButtonsHint);
    connect(ui.popDialogBtn, &QPushButton::clicked, this, [=]()
        {
            dlg->exec();
        });
    this->titleBar()->addAction(new QAction(tr("Test Button")));
    auto action = new QWidgetAction(this);
    action->setDefaultWidget(new QLineEdit(this));
    this->titleBar()->addAction(action);
    auto menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    menuBar->addMenu(tr("&File"))->addAction(tr("&Open"));
    this->setStatusBar(new QStatusBar(this));
    ui.popDialogBtn->winId();
    centralWidget()->winId();
}
