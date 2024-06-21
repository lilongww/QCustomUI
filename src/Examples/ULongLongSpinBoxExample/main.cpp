#include "ULongLongSpinBoxExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    ULongLongSpinBoxExample w;
    w.show();
    return a.exec();
}
