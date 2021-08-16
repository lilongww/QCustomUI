#include <QCustomUi/QCtmApplication.h>

#include "InputDialogExample.h"

int main(int agrc, char* agrv[])
{
    QCtmApplication a(agrc, agrv);
    InputDialogExample w;
    w.show();
    return a.exec();
}
