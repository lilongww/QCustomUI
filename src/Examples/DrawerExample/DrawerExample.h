#pragma once

#include <QCustomUi/QCtmWindow.h>

class DrawerExample : public QCtmWindow
{
    Q_OBJECT

public:
    DrawerExample(QWidget* parent = Q_NULLPTR);
    ~DrawerExample();

private:
    void init();
};
