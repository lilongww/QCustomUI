#pragma once

#include <QCustomUi/QCtmWindow.h>

class FlowChartExample : public QCtmWindow
{
    Q_OBJECT

public:
    FlowChartExample(QWidget* parent = Q_NULLPTR);
    ~FlowChartExample();

private:
    void init();

private:
    class QCtmFlowChart* m_chart;
};
