#include "EditLabelExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    EditLabelExample w;
    w.show();
    return a.exec();
}
