#include "ClassifyTreeViewExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    ClassifyTreeViewExample w;
    w.show();
    return a.exec();
}
