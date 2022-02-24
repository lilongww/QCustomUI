#include "EditLabel.h"

#include <QCustomUi/QCtmEditLabel.h>

EditLabel::EditLabel(QObject* parent /*= nullptr*/) : QObject(parent) {}

QString EditLabel::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmEditLabel\" name=\"editLabel\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* EditLabel::createWidget(QWidget* parent) { return new QCtmEditLabel(parent); }
