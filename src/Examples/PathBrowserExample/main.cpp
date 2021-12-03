#include "PathBrowserExample.h"

#include <QCustomUi/QCtmApplication.h>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);
    PathBrowserExample w;
    w.show();
    return a.exec();
}
