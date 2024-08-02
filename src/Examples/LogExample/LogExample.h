#pragma once

#include <QCustomUi/QCtmWindow.h>

class LogExample : public QCtmWindow
{
    Q_OBJECT

public:
    LogExample(QWidget* parent = nullptr);
    ~LogExample();

private:
    void init();
};
