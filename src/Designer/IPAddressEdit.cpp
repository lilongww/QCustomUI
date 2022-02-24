#include "IPAddressEdit.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

IPAddressEdit::IPAddressEdit(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool IPAddressEdit::isContainer() const { return false; }

bool IPAddressEdit::isInitialized() const { return m_initialized; }

QIcon IPAddressEdit::icon() const { return {}; }

QString IPAddressEdit::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmIPAddressEdit\" name=\"ipAddressEdit\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString IPAddressEdit::group() const { return "Input Widgets"; }

QString IPAddressEdit::includeFile() const { return "QCustomUi/QCtmIPAddressEdit.h"; }

QString IPAddressEdit::name() const { return "QCtmIPAddressEdit"; }

QString IPAddressEdit::toolTip() const { return "IPv4 editor."; }

QString IPAddressEdit::whatsThis() const { return "QCtmIPAddressEdit"; }

QWidget* IPAddressEdit::createWidget(QWidget* parent) { return new QCtmIPAddressEdit(parent); }

void IPAddressEdit::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
