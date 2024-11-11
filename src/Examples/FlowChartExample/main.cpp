#include "FlowChartExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    FlowChartExample w;
    w.showMaximized();
    return a.exec();
}
