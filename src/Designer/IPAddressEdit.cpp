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
#include "IPAddressEdit.h"

#include <QCustomUi/QCtmIPAddressEdit.h>

IPAddressEdit::IPAddressEdit(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool IPAddressEdit::isContainer() const { return false; }

bool IPAddressEdit::isInitialized() const { return m_initialized; }

QIcon IPAddressEdit::icon() const { return {}; }

QString IPAddressEdit::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmIPAddressEdit\" name=\"ipAddressEdit\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString IPAddressEdit::group() const { return "Input Widgets"; }

QString IPAddressEdit::includeFile() const { return "QCustomUi/QCtmIPAddressEdit.h"; }

QString IPAddressEdit::name() const { return "QCtmIPAddressEdit"; }

QString IPAddressEdit::toolTip() const { return "IPv4 editor."; }

QString IPAddressEdit::whatsThis() const { return "QCtmIPAddressEdit"; }

QWidget* IPAddressEdit::createWidget(QWidget* parent) { return new QCtmIPAddressEdit(parent); }

void IPAddressEdit::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
