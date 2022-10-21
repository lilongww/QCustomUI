#pragma once

#include "ui_ToolBoxExample.h"

class ToolBoxExample : public QWidget
{
    Q_OBJECT
public:
    ToolBoxExample(QWidget* parent = nullptr);
    ~ToolBoxExample();

private:
    Ui::ToolBoxExample ui;
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
