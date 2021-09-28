#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmTitleBar.h>
#include <QCustomUi/QCtmDialog.h>
#include <QCustomUi/QCtmLoadingDialog.h>

#include <QWidgetAction>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>

#include <thread>

WindowAndDialogExamples::WindowAndDialogExamples(QWidget* parent)
    : QCtmWindow(parent)
{
    ui.setupUi(this->centralWidget());
    auto* dlg = new QCtmDialog(this);
    auto layout = new QHBoxLayout(dlg->centralWidget());
    auto btn = new QPushButton(dlg);
    layout->addWidget(btn);
    btn->winId();
    dlg->setWindowFlag(Qt::WindowMinMaxButtonsHint);
    connect(ui.popDialogBtn, &QPushButton::clicked, this, [=]()
        {
            dlg->exec();
        });
    connect(btn, &QPushButton::clicked, this, [=]
        {
            QCtmLoadingDialog loading(dlg);
            std::thread thread([&]
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    QMetaObject::invokeMethod(&loading, "done");
                });
            loading.exec();
            thread.join();
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
