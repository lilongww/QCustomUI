#include "MultiPageViewExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    MultiPageViewExample w;
    w.show();
    return a.exec();
}