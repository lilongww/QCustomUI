#include "QCtmAbstractLegend.h"

struct QCtmAbstractLegend::Impl
{
};

QCtmAbstractLegend::QCtmAbstractLegend(QCtmAbstractChartView* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {}

QCtmAbstractLegend::~QCtmAbstractLegend() {}
