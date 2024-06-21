#pragma once

#include <QCustomUi/QCtmWindow.h>

class LongLongSpinBoxExample : public QCtmWindow
{
    Q_OBJECT

public:
    LongLongSpinBoxExample(QWidget* parent = Q_NULLPTR);
    ~LongLongSpinBoxExample();

private:
    void init();
};
