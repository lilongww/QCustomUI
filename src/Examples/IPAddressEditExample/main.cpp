#include "IPAddressEditExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    IPAddressEditExample w;
    w.show();
    return a.exec();
}
