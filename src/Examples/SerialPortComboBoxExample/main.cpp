#include "SerialPortComboBoxExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    SerialPortComboBoxExample w;
    w.show();
    return a.exec();
}
