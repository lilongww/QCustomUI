#include "CircleProgressBarExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    CircleProgressBarExample w;
    w.show();
    return a.exec();
}
