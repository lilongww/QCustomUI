#include "ClassifyTreeViewExample.h"

#include <QCustomUi/QCtmClassifyTreeItem.h>
#include <QCustomUi/QCtmClassifyTreeModel.h>
#include <QCustomUi/QCtmClassifyTreeView.h>

ClassifyTreeViewExample::ClassifyTreeViewExample(QWidget* parent) : QCtmWindow(parent)
{
    auto view = new QCtmClassifyTreeView(this);
    setCentralWidget(view);
    setWindowTitle("ClassifyTreeViewExample");
    auto model = new QCtmClassifyTreeModel(this);
    for (int i = 0; i < 5; i++)
    {
        auto root = new QCtmClassifyTreeGroupItem(QIcon(":/ClassifyTreeViewExample/resources/list.svg"), QString("root%1").arg(i));
        for (int j = 0; j < 20; j++)
            root->addChild(
                new QCtmClassifyTreeIconItem(QIcon(":/ClassifyTreeViewExample/resources/bus-color.svg"), QString("name%1").arg(j)));
        model->addItem(root);
    }
    view->setIconItemSize({ 70, 30 });
    view->setModel(model);
}
