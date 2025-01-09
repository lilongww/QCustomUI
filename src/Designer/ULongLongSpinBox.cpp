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
#include "ULongLongSpinBox.h"

#include <QCustomUi/QCtmULongLongSpinBox.h>

ULongLongSpinBox::ULongLongSpinBox(QObject* parent /*= nullptr*/) : QObject(parent) {}

bool ULongLongSpinBox::isContainer() const { return false; }

bool ULongLongSpinBox::isInitialized() const { return m_initialized; }

QIcon ULongLongSpinBox::icon() const { return {}; }

QString ULongLongSpinBox::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmULongLongSpinBox\" name=\"spinBox\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString ULongLongSpinBox::group() const { return "Input Widgets"; }

QString ULongLongSpinBox::includeFile() const { return "QCustomUi/QCtmULongLongSpinBox.h"; }

QString ULongLongSpinBox::name() const { return "QCtmULongLongSpinBox"; }

QString ULongLongSpinBox::toolTip() const { return "QCtmULongLongSpinBox"; }

QString ULongLongSpinBox::whatsThis() const { return "QCtmULongLongSpinBox"; }

QWidget* ULongLongSpinBox::createWidget(QWidget* parent) { return new QCtmULongLongSpinBox(parent); }

void ULongLongSpinBox::initialize(QDesignerFormEditorInterface* core)
{
    if (m_initialized)
        return;

    m_initialized = true;
}
