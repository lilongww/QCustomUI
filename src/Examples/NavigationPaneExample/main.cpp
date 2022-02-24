#include "NavigationPane.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    NavigationPane w;
    w.show();
    return a.exec();
}
