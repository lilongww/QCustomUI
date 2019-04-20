#include "OperationTree.h"

OperationTree::OperationTree(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.treeWidget->expandAll();
    ui.treeWidget->header()->hide();
}

OperationTree::~OperationTree()
{
}
