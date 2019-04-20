#pragma once

#include <QWidget>
#include "ui_ProjectTree.h"

class ProjectTree : public QWidget
{
    Q_OBJECT

public:
    ProjectTree(QWidget *parent = Q_NULLPTR);
    ~ProjectTree();

private:
    Ui::ProjectTree ui;
};
