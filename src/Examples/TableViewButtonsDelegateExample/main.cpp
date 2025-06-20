#include "TableViewButtonsDelegateExample.h"

#include <QCustomUi/QCtmApplication.h>
#include <QCustomUi/QCtmStyleSheet.h>
#include <QCustomUi/QCtmWindow.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    TableViewButtonsDelegateExample w;
    w.show();
    return a.exec();
}
