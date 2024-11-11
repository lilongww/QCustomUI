#include "FlowChartExample.h"

#include <QCustomUi/QCtmFlowChart.h>

#include <QHBoxLayout>

FlowChartExample::FlowChartExample(QWidget* parent) : QCtmWindow(parent) { init(); }

FlowChartExample::~FlowChartExample() {}

void FlowChartExample::init()
{
    auto layout = new QHBoxLayout(this->centralWidget());
    m_chart     = new QCtmFlowChart(this);
    layout->addWidget(m_chart);
}
