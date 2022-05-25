#pragma once

#include "qcustomui_global.h"

#include <QObject>

class QCtmAbstractChartView;
class QCUSTOMUI_EXPORT QCtmAbstractSeries : public QObject
{
    Q_OBJECT
public:
    QCtmAbstractSeries(QCtmAbstractChartView* parent);
    ~QCtmAbstractSeries();

    void setName(const QString& name);
    const QString& name() const;
    void setVisible(bool visible);
    bool isVisible() const;
    QCtmAbstractChartView* chart() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};