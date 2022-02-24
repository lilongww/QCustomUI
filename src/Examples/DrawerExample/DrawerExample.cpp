#include "DrawerExample.h"

#include <QCustomUi/QCtmDrawerItemWidget.h>
#include <QCustomUi/QCtmDrawerWidget.h>
#include <QHBoxLayout>

#include <QDir>
#include <QFileSystemModel>
#include <QLabel>
#include <QTreeView>

DrawerExample::DrawerExample(QWidget* parent) : QCtmWindow(parent) { init(); }

DrawerExample::~DrawerExample() {}

void DrawerExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    auto drawer = new QCtmDrawerWidget(this);
    layout->addWidget(drawer);

    auto tree  = new QTreeView(this);
    auto model = new QFileSystemModel(this);
    model->setRootPath(QDir::homePath());
    tree->setModel(model);

    auto item1 = drawer->addWidget(tr("Example1"), new QLabel(tr("Example text")));
    item1->addAction(QIcon(":/DrawerExample/Resources/Edit.svg"));
    drawer->addWidget(tr("Example2"), tree);
    // drawer->setExclusive(true);
}
