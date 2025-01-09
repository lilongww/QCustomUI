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
#include "MultiPageButtonBox.h"

#include <QCustomUi/QCtmMultiPageButtonBox.h>

MultiPageButtonBox::MultiPageButtonBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool MultiPageButtonBox::isContainer() const { return false; }

bool MultiPageButtonBox::isInitialized() const { return m_initialized; }

QIcon MultiPageButtonBox::icon() const { return {}; }

QString MultiPageButtonBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmMultiPageButtonBox\" name=\"multiPageButtonBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString MultiPageButtonBox::group() const { return "Item Views (Model-Based)"; }

QString MultiPageButtonBox::includeFile() const { return "QCustomUi/QCtmMultiPageButtonBox.h"; }

QString MultiPageButtonBox::name() const { return "QCtmMultiPageButtonBox"; }

QString MultiPageButtonBox::toolTip() const { return {}; }

QString MultiPageButtonBox::whatsThis() const { return {}; }

QWidget* MultiPageButtonBox::createWidget(QWidget* parent) { return new QCtmMultiPageButtonBox(parent); }

void MultiPageButtonBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
