#include "MultiComboBox.h"

#include <QCustomUi/QCtmMultiComboBox.h>

MultiComboBox::MultiComboBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

QIcon MultiComboBox::icon() const { return {}; }

QString MultiComboBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmMultiComboBox\" name=\"multiComboBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* MultiComboBox::createWidget(QWidget* parent) { return new QCtmMultiComboBox(parent); }

void MultiComboBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
