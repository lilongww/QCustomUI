#pragma once

#include <QWidget>
#include "ui_OperationTree.h"

class OperationTree : public QWidget
{
    Q_OBJECT

public:
    OperationTree(QWidget *parent = Q_NULLPTR);
    ~OperationTree();

private:
    Ui::OperationTree ui;
};
