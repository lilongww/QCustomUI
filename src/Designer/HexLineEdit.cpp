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
#include "HexLineEdit.h"

#include <QCustomUi/QCtmHexLineEdit.h>

HexLineEdit::HexLineEdit(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool HexLineEdit::isContainer() const { return false; }

bool HexLineEdit::isInitialized() const { return m_initialized; }

QIcon HexLineEdit::icon() const { return {}; }

QString HexLineEdit::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmHexLineEdit\" name=\"hexLineEdit\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString HexLineEdit::group() const { return "Input Widgets"; }

QString HexLineEdit::includeFile() const { return "QCustomUi/QCtmHexLineEdit.h"; }

QString HexLineEdit::name() const { return "QCtmHexLineEdit"; }

QString HexLineEdit::toolTip() const { return "QCtmHexLineEdit"; }

QString HexLineEdit::whatsThis() const { return "QCtmHexLineEdit"; }

QWidget* HexLineEdit::createWidget(QWidget* parent) { return new QCtmHexLineEdit(parent); }

void HexLineEdit::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
