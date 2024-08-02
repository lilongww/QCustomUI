#include "LogExample.h"

#include <QCustomUi/QCtmLogManager.h>
#include <QCustomUi/QCtmLogWidget.h>
#include <QCustomUi/QCtmTabPage.h>
#include <QCustomUi/QCtmTabWidget.h>

#include <QDebug>
#include <QImage>
#include <QToolButton>
#include <QVBoxLayout>

/* XPM */
static const char* xpmImage[] = {
    /* columns rows colors chars-per-pixel */
    "16 16 5 1 ",
    "  c None",
    ". c #313131",
    "X c #323232",
    "o c gray20",
    "O c #343434",
    /* pixels */
    "                ",
    "   oo    o      ",
    " oooooooooooo   ",
    " o oo      oo   ",
    " o         oo   ",
    " o oooooooooo   ",
    " o         oo   ",
    " o        oOo   ",
    " o oooo oo  oo  ",
    " o     oO     o ",
    " o     o   o  o ",
    " o    oo   oo o ",
    " o     o   o  o ",
    " ooooooOoo   o  ",
    "         oooo   ",
    "                "
};

LogExample::LogExample(QWidget* parent) : QCtmWindow(parent) { init(); }

LogExample::~LogExample() {}

void LogExample::init()
{
    auto layout = new QVBoxLayout(centralWidget());
    layout->addWidget(new QWidget(this), 1);

    auto tabWidget = new QCtmTabWidget(this); // [2] add QCtmTabWidget
    layout->addWidget(tabWidget);
    auto logWidget = new QCtmLogWidget("MyLog", this);
    auto page      = tabWidget->addTab(logWidget, "Log"); // [3] add QCtmLogWidget
    page->addActions(logWidget->actions());               // [4] add QCtmLogWidget control action.
    auto btn = new QToolButton(this);
    this->statusBar()->addPermanentWidget(btn);
    btn->setIcon(QPixmap::fromImage(QImage(xpmImage)));
    connect(btn, &QToolButton::clicked, this, [=]() { tabWidget->setVisible(!tabWidget->isVisible()); });

    // [5] show your logs
    // show on QCtmLogWidget and write to log file.
    qInfo() << "#MyLog"
            << "Info message";
    qWarning() << "#MyLog"
               << "Warning message";
    qCritical() << "#MyLog"
                << "Error message";

    // only write to log file.
    qDebug() << "Debug message";
    qInfo() << "Info message 2";
}
