#pragma once

#include "QCtmAbstractSeries.h"

#include <QObject>

class QCtmRadarChartView;
class QCUSTOMUI_EXPORT QCtmRadarSeries : public QCtmAbstractSeries
{
    Q_OBJECT

public:
    QCtmRadarSeries(QCtmRadarChartView* parent);
    ~QCtmRadarSeries();
    void setValues(const std::vector<int>& values);
    const std::vector<int>& values() const;
    void setBrush(const QBrush& brush);
    const QBrush& brush() const;
    void setBorderColor(const QColor& color);
    const QColor& borderColor() const;
signals:
    void valueChanged();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
