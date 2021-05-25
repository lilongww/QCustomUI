#include "QCustomUiWidgets.h"
#include "StageProgressBar.h"
#include "IPAddressEdit.h"
#include "MultiComboBox.h"
#include "ComboBox.h"
#include "TableView.h"
#include "ClassifyTreeView.h"
#include "EditLabel.h"

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
    m_impl->widgets.push_back(new IPAddressEdit(this));
    m_impl->widgets.push_back(new MultiComboBox(this));
    m_impl->widgets.push_back(new ComboBox(this));
    m_impl->widgets.push_back(new TableView(this));
    m_impl->widgets.push_back(new ClassifyTreeView(this));
    m_impl->widgets.push_back(new EditLabel(this));
    qApp->setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
}

QCustomUiWidgets::~QCustomUiWidgets()
{

}

QList<QDesignerCustomWidgetInterface*> QCustomUiWidgets::customWidgets() const
{
    return m_impl->widgets;
}
