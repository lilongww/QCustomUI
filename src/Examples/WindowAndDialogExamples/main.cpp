#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmApplication.h>

#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QDialog>
#include <QStyle>

#include <qt_windows.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    a.setWindowIcon(a.style()->standardIcon(QStyle::SP_FileIcon));
    WindowAndDialogExamples w;
    w.showMaximized();
    return a.exec();
}
