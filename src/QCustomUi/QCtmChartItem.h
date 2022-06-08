#pragma once

#include "qcustomui_global.h"

#include <QObject>

class QCtmAbstractChartView;
class QCUSTOMUI_EXPORT QCtmChartItem : public QObject
{
    Q_OBJECT

public:
    QCtmChartItem(QCtmAbstractChartView* parent);
    ~QCtmChartItem();

    virtual QCtmAbstractChartView* chart() const;

protected:
    virtual void draw(QPainter* p) = 0;
};
