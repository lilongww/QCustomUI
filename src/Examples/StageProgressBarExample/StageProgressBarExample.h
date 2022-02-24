#pragma once

#include <QCustomUi/QCtmWindow.h>

class StageProgressBarExample : public QCtmWindow
{
    Q_OBJECT

public:
    StageProgressBarExample(QWidget* parent = Q_NULLPTR);
    ~StageProgressBarExample();

private:
    void init();
};
