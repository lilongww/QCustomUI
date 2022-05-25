#include "RadarChartViewExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    RadarChartViewExample w;
    w.show();
    return a.exec();
}
