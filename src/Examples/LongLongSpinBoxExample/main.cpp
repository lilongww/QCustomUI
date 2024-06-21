#include "LongLongSpinBoxExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    LongLongSpinBoxExample w;
    w.show();
    return a.exec();
}
