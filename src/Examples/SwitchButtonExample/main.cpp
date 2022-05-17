#include "SwitchButtonExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    SwitchButtonExample w;
    w.show();
    return a.exec();
}
