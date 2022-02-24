#pragma once

#include "ui_ToolBoxExample.h"

class ToolBoxExample : public QWidget
{
public:
    ToolBoxExample(QWidget* parent = nullptr);
    ~ToolBoxExample();

private:
    Ui::ToolBoxExample ui;
};
