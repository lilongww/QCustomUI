#pragma once

#include <QCustomUi/QCtmWindow.h>

class MultiComboBoxExample : public QCtmWindow
{
    Q_OBJECT

public:
    MultiComboBoxExample(QWidget* parent = Q_NULLPTR);
    ~MultiComboBoxExample();

private:
    void init();
};
