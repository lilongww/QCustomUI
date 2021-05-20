#include "StageProgressBar.h"

#include <QCustomUi/QCtmStageProgressBar.h>

StageProgressBar::StageProgressBar(QObject* parent /*= nullptr*/)
    : QObject(parent)
{

}

bool StageProgressBar::isContainer() const
{
    return false;
}

bool StageProgressBar::isInitialized() const
{
    return m_initialized;
}

QIcon StageProgressBar::icon() const
{
    return {};
}

QString StageProgressBar::domXml() const
{
    return "<ui language=\"c++\">\n"
        " <widget class=\"StageProgressBar\" name=\"stageProgressBar\">\n"
        "  <property name=\"geometry\">\n"
        "   <rect>\n"
        "    <x>0</x>\n"
        "    <y>0</y>\n"
        "    <width>100</width>\n"
        "    <height>100</height>\n"
        "   </rect>\n"
        "  </property>\n"
        "  <property name=\"toolTip\" >\n"
        "   <string>The current time</string>\n"
        "  </property>\n"
        "  <property name=\"whatsThis\" >\n"
        "   <string>StageProgressBar.</string>\n"
        "  </property>\n"
        " </widget>\n"
        "</ui>\n";
}

QString StageProgressBar::group() const
{
    return "Display Widgets";
}

QString StageProgressBar::includeFile() const
{
    return "QCustomUi/QCtmStageProgressBar.h";
}

QString StageProgressBar::name() const
{
    return "QCtmStageProgressBar";
}

QString StageProgressBar::toolTip() const
{
    return {};
}

QString StageProgressBar::whatsThis() const
{
    return {};
}

QWidget* StageProgressBar::createWidget(QWidget* parent)
{
    return new QCtmStageProgressBar(parent);
}

void StageProgressBar::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
