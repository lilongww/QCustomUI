#include "RecentView.h"

#include <QCustomUi/QCtmRecentView.h>

RecentView::RecentView(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool RecentView::isContainer() const { return false; }

bool RecentView::isInitialized() const { return m_initialized; }

QIcon RecentView::icon() const { return {}; }

QString RecentView::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmRecentView\" name=\"recentView\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString RecentView::group() const { return "Item Views (Model-Based)"; }

QString RecentView::includeFile() const { return "QCustomUi/QCtmRecentView.h"; }

QString RecentView::name() const { return "QCtmRecentView"; }

QString RecentView::toolTip() const { return {}; }

QString RecentView::whatsThis() const { return {}; }

QWidget* RecentView::createWidget(QWidget* parent) { return new QCtmRecentView(parent); }

void RecentView::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
