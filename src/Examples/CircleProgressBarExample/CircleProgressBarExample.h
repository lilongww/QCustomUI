#pragma once

#include <QCustomUi/QCtmWindow.h>

class CircleProgressBarExample : public QCtmWindow
{
    Q_OBJECT

public:
    CircleProgressBarExample(QWidget* parent = Q_NULLPTR);
    ~CircleProgressBarExample();

private:
    void init();
};
