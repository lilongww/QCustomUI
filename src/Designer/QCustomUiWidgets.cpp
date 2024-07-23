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
#include "QCustomUiWidgets.h"
#include "CircleProgressBar.h"
#include "ClassifyTreeView.h"
#include "ComboBox.h"
#include "EditLabel.h"
#include "HexLineEdit.h"
#include "IPAddressEdit.h"
#include "LongLongSpinBox.h"
#include "MultiComboBox.h"
#include "RecentView.h"
#include "SelectionButtonBox.h"
#include "StageProgressBar.h"
#include "SwitchButton.h"
#include "TableView.h"
#include "ULongLongSpinBox.h"

#include <QCustomUi/QCtmStyleSheet.h>

#include <QApplication>

struct QCustomUiWidgets::Impl
{
    QList<QDesignerCustomWidgetInterface*> widgets;
};

QCustomUiWidgets::QCustomUiWidgets(QObject* parent /*= 0*/) : QObject(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->widgets.push_back(new StageProgressBar(this));
    m_impl->widgets.push_back(new IPAddressEdit(this));
    m_impl->widgets.push_back(new MultiComboBox(this));
    m_impl->widgets.push_back(new ComboBox(this));
    m_impl->widgets.push_back(new TableView(this));
    m_impl->widgets.push_back(new ClassifyTreeView(this));
    m_impl->widgets.push_back(new EditLabel(this));
    m_impl->widgets.push_back(new SwitchButton(this));
    m_impl->widgets.push_back(new CircleProgressBar(this));
    m_impl->widgets.push_back(new RecentView(this));
    m_impl->widgets.push_back(new SelectionButtonBox(this));
    m_impl->widgets.push_back(new LongLongSpinBox(this));
    m_impl->widgets.push_back(new ULongLongSpinBox(this));
    m_impl->widgets.push_back(new HexLineEdit(this));

    qApp->setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
}

QCustomUiWidgets::~QCustomUiWidgets() {}

QList<QDesignerCustomWidgetInterface*> QCustomUiWidgets::customWidgets() const { return m_impl->widgets; }
