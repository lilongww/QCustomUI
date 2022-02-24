#include "StageProgressBarExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    StageProgressBarExample w;
    w.show();
    return a.exec();
}
