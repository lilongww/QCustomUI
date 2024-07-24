#pragma once

#include <QCustomUi/QCtmWindow.h>

class SerialPortComboBoxExample : public QCtmWindow
{
    Q_OBJECT

public:
    SerialPortComboBoxExample(QWidget* parent = Q_NULLPTR);
    ~SerialPortComboBoxExample();

private:
    void init();
};
