#include "QCustomUiWidgets.h"
#include "StageProgressBar.h"

#include <QCustomUi/QCtmStyleSheet.h>

#include <QApplication>

struct QCustomUiWidgets::Impl
{
    QList<QDesignerCustomWidgetInterface*> widgets;
};

QCustomUiWidgets::QCustomUiWidgets(QObject* parent /*= 0*/)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{

    m_impl->widgets.push_back(new StageProgressBar(this));
    qApp->setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
}

QList<QDesignerCustomWidgetInterface*> QCustomUiWidgets::customWidgets() const
{
    return m_impl->widgets;
}
