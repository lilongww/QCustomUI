#include "DisplayWidgetExamples.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char *argv[])
{
    QCtmApplication a(argc, argv);
    DisplayWidgetExamples w;
    w.show();
    return a.exec();
}
