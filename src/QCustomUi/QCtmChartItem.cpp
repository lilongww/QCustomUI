#include "QCtmChartItem.h"

QCtmChartItem::QCtmChartItem(QCtmAbstractChartView* parent)
    : QObject(parent)
{
}

QCtmChartItem::~QCtmChartItem()
{
}

QCtmAbstractChartView* QCtmChartItem::chart() const { return qobject_cast<QCtmAbstractChartView*>(parent()); }
