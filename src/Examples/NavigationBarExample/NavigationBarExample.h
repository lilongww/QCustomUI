#pragma once

#include <QCustomUi/QCtmWindow.h>

class NavigationBarExample : public QCtmWindow
{
    Q_OBJECT

public:
    NavigationBarExample(QWidget* parent = Q_NULLPTR);
    ~NavigationBarExample();

private:
    void init();
};
