#include "DigitKeyboardExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    DigitKeyboardExample w;
    w.show();
    return a.exec();
}
