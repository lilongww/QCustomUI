#include "StageProgressBar.h"

#include <QCustomUi/QCtmStageProgressBar.h>

StageProgressBar::StageProgressBar(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool StageProgressBar::isContainer() const { return false; }

bool StageProgressBar::isInitialized() const { return m_initialized; }

QIcon StageProgressBar::icon() const { return {}; }

QString StageProgressBar::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmStageProgressBar\" name=\"stageProgressBar\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString StageProgressBar::group() const { return "Display Widgets"; }

QString StageProgressBar::includeFile() const { return "QCustomUi/QCtmStageProgressBar.h"; }

QString StageProgressBar::name() const { return "QCtmStageProgressBar"; }

QString StageProgressBar::toolTip() const { return {}; }

QString StageProgressBar::whatsThis() const { return {}; }

QWidget* StageProgressBar::createWidget(QWidget* parent) { return new QCtmStageProgressBar(parent); }

void StageProgressBar::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
