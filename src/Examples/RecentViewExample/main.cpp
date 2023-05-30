#include "RecentViewExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    RecentViewExample w;
    w.show();
    return a.exec();
}
