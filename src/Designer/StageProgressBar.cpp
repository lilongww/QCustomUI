/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/
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
