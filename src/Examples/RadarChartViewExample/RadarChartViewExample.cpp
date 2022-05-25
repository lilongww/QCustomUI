#include "RadarChartViewExample.h"

#include <QCustomUi/QCtmRadarChartView.h>
#include <QCustomUi/QCtmRadarSeries.h>

#include <QVBoxLayout>

RadarChartViewExample::RadarChartViewExample(QWidget* parent /*= nullptr*/) : QCtmWindow(parent)
{
    auto layout      = new QVBoxLayout(this->centralWidget());
    auto graphWidget = new QCtmRadarChartView(this);
    layout->addWidget(graphWidget);
    graphWidget->setGridType(QCtmRadarChartView::GridType::Polygon);
    graphWidget->setGridMargins({ 100, 100, 100, 100 });
    graphWidget->setIndicator({ { QStringLiteral("语文"), { 0, 100 } },
                                { QStringLiteral("数学"), { 0, 100 } },
                                { QStringLiteral("英语"), { 0, 100 } },
                                { QStringLiteral("物理"), { 0, 100 } },
                                { QStringLiteral("化学"), { 0, 100 } },
                                { QStringLiteral("生物"), { 0, 100 } } });
    auto graph1 = graphWidget->addSeries();
    graph1->setValues({ 24, 40, 100, 50, 15, 100 });

    auto graph2 = graphWidget->addSeries();
    graph2->setValues({ 70, 20, 50, 100, 75, 35 });

    auto graph3 = graphWidget->addSeries();
    graph3->setValues({ 60, 100, 32, 44, 65, 12 });
}

RadarChartViewExample::~RadarChartViewExample() {}
