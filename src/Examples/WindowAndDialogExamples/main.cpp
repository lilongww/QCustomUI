#include "WindowAndDialogExamples.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char *argv[])
{
    QCtmApplication a(argc, argv);
    WindowAndDialogExamples w;
    w.show();
    return a.exec();
}
