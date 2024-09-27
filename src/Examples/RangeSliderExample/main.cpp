#include "RangeSliderExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    RangeSliderExample w;
    w.show();
    return a.exec();
}
