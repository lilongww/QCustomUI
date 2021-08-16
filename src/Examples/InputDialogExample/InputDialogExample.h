#pragma once

#include "ui_InputDialogExample.h"

#include <QCustomUi/QCtmWindow.h>

class InputDialogExample : public QCtmWindow
{
    Q_OBJECT
public:
    InputDialogExample(QWidget* parent = nullptr);
    ~InputDialogExample();

private:
    Ui::InputDialogExample ui;
};
