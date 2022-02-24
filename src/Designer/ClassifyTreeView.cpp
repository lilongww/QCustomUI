#include "ClassifyTreeView.h"

#include <QCustomUi/QCtmClassifyTreeView.h>

ClassifyTreeView::ClassifyTreeView(QObject* parent /*= nullptr*/) : QObject(parent) {}

QString ClassifyTreeView::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmClassifyTreeView\" name=\"classifyTreeView\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* ClassifyTreeView::createWidget(QWidget* parent) { return new QCtmClassifyTreeView(parent); }
