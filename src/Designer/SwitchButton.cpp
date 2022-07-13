#include "SwitchButton.h"

#include <QCustomUi/QCtmSwitchButton.h>

SwitchButton::SwitchButton(QObject* parent /*= nullptr*/) : QObject(parent = nullptr) {}

QString SwitchButton::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmSwitchButton\" name=\"switchButton\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* SwitchButton::createWidget(QWidget* parent) { return new QCtmSwitchButton(parent); }
