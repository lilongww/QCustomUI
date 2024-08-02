#include "LogExample.h"

#include <QCustomUi/QCtmApplication.h>
#include <QCustomUi/QCtmLogManager.h>

int main(int argc, char* argv[])
{
    QCtmLogManager::initBeforeApp(); // [1] init before QCtmApplication
    QCtmApplication a(argc, argv);
    LogExample w;
    w.showMaximized();
    return a.exec();
}
