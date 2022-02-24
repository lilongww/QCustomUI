#include "TableView.h"

#include <QCustomUi/QCtmTableView.h>

TableView::TableView(QObject* parent /*= nullptr*/) : QObject(parent = nullptr) {}

QString TableView::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmTableView\" name=\"tableView\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* TableView::createWidget(QWidget* parent) { return new QCtmTableView(parent); }
