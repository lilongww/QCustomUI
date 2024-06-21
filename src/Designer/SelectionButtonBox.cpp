/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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
#include "SelectionButtonBox.h"

#include <QCustomUi/QCtmSelectionButtonBox.h>

SelectionButtonBox::SelectionButtonBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool SelectionButtonBox::isContainer() const { return false; }

bool SelectionButtonBox::isInitialized() const { return m_initialized; }

QIcon SelectionButtonBox::icon() const { return {}; }

QString SelectionButtonBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmSelectionButtonBox\" name=\"SelectionButtonBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString SelectionButtonBox::group() const { return "Buttons"; }

QString SelectionButtonBox::includeFile() const { return "QCustomUi/QCtmSelectionButtonBox.h"; }

QString SelectionButtonBox::name() const { return "QCtmSelectionButtonBox"; }

QString SelectionButtonBox::toolTip() const { return {}; }

QString SelectionButtonBox::whatsThis() const { return {}; }

QWidget* SelectionButtonBox::createWidget(QWidget* parent) { return new QCtmSelectionButtonBox(Qt::Horizontal, parent); }

void SelectionButtonBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
