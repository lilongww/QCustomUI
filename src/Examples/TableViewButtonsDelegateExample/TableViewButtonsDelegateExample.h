#pragma once

#include "ui_TableViewButtonsDelegateExample.h"

#include <QCustomUi/QCtmWindow.h>

class TableViewButtonsDelegateExample : public QCtmWindow
{
    Q_OBJECT
public:
    explicit TableViewButtonsDelegateExample(QWidget* parent = nullptr);
    ~TableViewButtonsDelegateExample();

private:
    Ui::TableViewButtonsDelegateExample ui;
};