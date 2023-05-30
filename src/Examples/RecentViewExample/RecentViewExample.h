#pragma once

#include <QCustomUi/QCtmWindow.h>

class RecentViewExample : public QCtmWindow
{
    Q_OBJECT

public:
    RecentViewExample(QWidget* parent = Q_NULLPTR);
    ~RecentViewExample();

private:
    void init();
};
