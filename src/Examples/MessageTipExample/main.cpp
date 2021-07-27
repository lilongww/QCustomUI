#include "MessageTipExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int agrc, char* agrv[])
{
    QCtmApplication a(agrc, agrv);
    MessageTipExample w;
    w.show();
    return a.exec();
}
