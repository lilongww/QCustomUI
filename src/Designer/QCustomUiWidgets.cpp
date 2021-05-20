#include "QCustomUiWidgets.h"
#include "StageProgressBar.h"

struct QCustomUiWidgets::Impl
{
    QList<QDesignerCustomWidgetInterface*> widgets;
};

QCustomUiWidgets::QCustomUiWidgets(QObject* parent /*= 0*/)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->widgets.push_back(new StageProgressBar(this));
}

QList<QDesignerCustomWidgetInterface*> QCustomUiWidgets::customWidgets() const
{
    return m_impl->widgets;
}
