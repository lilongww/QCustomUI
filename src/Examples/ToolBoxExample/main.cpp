#include "ToolBoxExample.h"

#include <QCustomUi/QCtmApplication.h>
#include <QCustomUi/QCtmToolBox.h>
#include <QCustomUi/QCtmWindow.h>

#include <QHBoxLayout>
#include <QSplitter>

int main(int argc, char* argv[])
{
    QCtmApplication a(argc, argv);

    QCtmToolBox* toolBox = new QCtmToolBox;
    ToolBoxExample* w    = new ToolBoxExample(toolBox);
    toolBox->addWidget(QObject::tr("Explorer"), w);
    QCtmWindow window;
    QSplitter* sp = new QSplitter(&window);
    auto layout   = new QHBoxLayout(window.centralWidget());
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(sp);
    sp->addWidget(toolBox);
    auto content = new QWidget(sp);
    sp->addWidget(content);
    // auto edit = new QLineEdit(content);
    sp->setStretchFactor(1, 1);
    window.show();
    return a.exec();
}
