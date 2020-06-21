#include "DrawerExample.h"

#include <QCustomUi/QCtmDrawerWidget.h>
#include <QHBoxLayout>

#include <QLabel>
#include <QFileSystemModel>
#include <QTreeView>

DrawerExample::DrawerExample(QWidget *parent)
    : QCtmWindow(parent)
{
    init();
}

DrawerExample::~DrawerExample()
{

}

void DrawerExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    auto drawer = new QCtmDrawerWidget(this);
    layout->addWidget(drawer);

    auto tree = new QTreeView(this);
    tree->setModel(new QFileSystemModel(this));

    drawer->addWidget(tr("Example1"), new QLabel(tr("Example text")));
    drawer->addWidget(tr("Example2"), tree);
    //drawer->setExclusive(true);
}
