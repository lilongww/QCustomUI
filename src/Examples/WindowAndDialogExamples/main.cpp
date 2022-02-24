#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmApplication.h>

#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QDialog>

#include <qt_windows.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    WindowAndDialogExamples w;
    w.showMaximized();
    return a.exec();
}
