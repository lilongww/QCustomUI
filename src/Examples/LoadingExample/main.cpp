#include "LoadingExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    LoadingExample w;
    w.show();
    return a.exec();
}
