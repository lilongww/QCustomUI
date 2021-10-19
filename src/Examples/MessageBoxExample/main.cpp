#include "MessageBoxExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    MessageBoxExample w;
    w.show();
    return a.exec();
}
