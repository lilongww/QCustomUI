#pragma once

#include <QCustomUi/QCtmWindow.h>

class NavigationPane : public QCtmWindow
{
    Q_OBJECT

public:
    NavigationPane(QWidget* parent = Q_NULLPTR);
    ~NavigationPane();

private:
    void init();
};
