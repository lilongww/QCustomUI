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
#include "SerialPortComboBox.h"

#include <QCustomUi/QCtmSerialPortComboBox.h>

SerialPortComboBox::SerialPortComboBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

QIcon SerialPortComboBox::icon() const { return {}; }

QString SerialPortComboBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmSerialPortComboBox\" name=\"serialPortComboBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* SerialPortComboBox::createWidget(QWidget* parent) { return new QCtmSerialPortComboBox(parent); }

void SerialPortComboBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
