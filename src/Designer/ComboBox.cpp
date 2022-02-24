#include "ComboBox.h"

#include <QCustomUi/QCtmComboBox.h>

ComboBox::ComboBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

QString ComboBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmComboBox\" name=\"comboBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* ComboBox::createWidget(QWidget* parent) { return new QCtmComboBox(parent); }
