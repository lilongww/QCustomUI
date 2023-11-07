#include "SelectionButtonBox.h"

#include <QCustomUi/QCtmSelectionButtonBox.h>

SelectionButtonBox::SelectionButtonBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool SelectionButtonBox::isContainer() const { return false; }

bool SelectionButtonBox::isInitialized() const { return m_initialized; }

QIcon SelectionButtonBox::icon() const { return {}; }

QString SelectionButtonBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmSelectionButtonBox\" name=\"SelectionButtonBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString SelectionButtonBox::group() const { return "Buttons"; }

QString SelectionButtonBox::includeFile() const { return "QCustomUi/QCtmSelectionButtonBox.h"; }

QString SelectionButtonBox::name() const { return "QCtmSelectionButtonBox"; }

QString SelectionButtonBox::toolTip() const { return {}; }

QString SelectionButtonBox::whatsThis() const { return {}; }

QWidget* SelectionButtonBox::createWidget(QWidget* parent) { return new QCtmSelectionButtonBox(Qt::Horizontal, parent); }

void SelectionButtonBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
