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
#include "LongLongSpinBox.h"

#include <QCustomUi/QCtmLongLongSpinBox.h>

LongLongSpinBox::LongLongSpinBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool LongLongSpinBox::isContainer() const { return false; }

bool LongLongSpinBox::isInitialized() const { return m_initialized; }

QIcon LongLongSpinBox::icon() const { return {}; }

QString LongLongSpinBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmLongLongSpinBox\" name=\"spinBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString LongLongSpinBox::group() const { return "Input Widgets"; }

QString LongLongSpinBox::includeFile() const { return "QCustomUi/QCtmLongLongSpinBox.h"; }

QString LongLongSpinBox::name() const { return "QCtmLongLongSpinBox"; }

QString LongLongSpinBox::toolTip() const { return "QCtmLongLongSpinBox"; }

QString LongLongSpinBox::whatsThis() const { return "QCtmLongLongSpinBox"; }

QWidget* LongLongSpinBox::createWidget(QWidget* parent) { return new QCtmLongLongSpinBox(parent); }

void LongLongSpinBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
