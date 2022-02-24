#pragma once

#include "ui_MessageBoxExample.h"

#include <QCustomUi/QCtmWindow.h>

class MessageBoxExample : public QCtmWindow
{
    Q_OBJECT

public:
    MessageBoxExample(QWidget* parent = Q_NULLPTR);
    ~MessageBoxExample();

private:
    void init();

private:
    Ui::MessageBoxExampleClass ui;
};
