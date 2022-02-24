#include "DrawerExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    DrawerExample w;
    w.show();
    return a.exec();
}
