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
