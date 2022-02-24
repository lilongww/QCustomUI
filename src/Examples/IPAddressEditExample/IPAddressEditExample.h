#pragma once

#include <QCustomUi/QCtmWindow.h>

class IPAddressEditExample : public QCtmWindow
{
    Q_OBJECT

public:
    IPAddressEditExample(QWidget* parent = Q_NULLPTR);
    ~IPAddressEditExample();

private:
    void init();
};
