#pragma once

#include <QCustomUi/QCtmWindow.h>

class ULongLongSpinBoxExample : public QCtmWindow
{
    Q_OBJECT

public:
    ULongLongSpinBoxExample(QWidget* parent = Q_NULLPTR);
    ~ULongLongSpinBoxExample();

private:
    void init();
};
