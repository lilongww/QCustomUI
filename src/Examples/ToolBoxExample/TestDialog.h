#pragma once

#include "ui_TestDialog.h"

#include <QCustomUi/QCtmDialog.h>

class TestDialog: public QCtmDialog
{
    Q_OBJECT
public:
    TestDialog(QWidget* parent = Q_NULLPTR);
    ~TestDialog();

private:
    Ui::TestDialog ui;
};
