#include "QCtmAbstractSeries.h"
#include "QCtmAbstractChartView.h"

struct QCtmAbstractSeries::Impl
{
    QString name;
    bool visible { true };
};

QCtmAbstractSeries::QCtmAbstractSeries(QCtmAbstractChartView* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {}

QCtmAbstractSeries::~QCtmAbstractSeries() {}

void QCtmAbstractSeries::setName(const QString& name) { m_impl->name = name; }

const QString& QCtmAbstractSeries::name() const { return m_impl->name; }

void QCtmAbstractSeries::setVisible(bool visible) { m_impl->visible = visible; }

bool QCtmAbstractSeries::isVisible() const { return m_impl->visible; }

QCtmAbstractChartView* QCtmAbstractSeries::chart() const { return qobject_cast<QCtmAbstractChartView*>(parent()); }
