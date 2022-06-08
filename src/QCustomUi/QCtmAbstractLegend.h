#pragma once

#include "QCtmChartItem.h"

#include <QObject>

class QCtmAbstractChartView;
class QCUSTOMUI_EXPORT QCtmAbstractLegend : public QCtmChartItem
{
    Q_OBJECT

public:
    QCtmAbstractLegend(QCtmAbstractChartView* parent);
    ~QCtmAbstractLegend();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
