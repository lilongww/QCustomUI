#include "ProjectTree.h"

ProjectTree::ProjectTree(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.treeWidget->expandAll();
    ui.treeWidget->header()->hide();
}

ProjectTree::~ProjectTree()
{
}
