#pragma once

#include <QCustomUi/QCtmWindow.h>

class HexLineEditExample : public QCtmWindow
{
    Q_OBJECT

public:
    HexLineEditExample(QWidget* parent = Q_NULLPTR);
    ~HexLineEditExample();

private:
    void init();
};
