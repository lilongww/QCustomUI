/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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

#include "QCtmComboBox.h"

#include <QListView>

/*!
    \class      QCtmComboBox
    \brief      提供一个下拉列表可被样式表设置的combobox，以及其他功能补丁.
    \inherits   QComboBox
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \sa         QComboBox
    \inheaderfile QCtmComboBox.h
*/

/*!
    \brief      构造一个父窗口为 \a parent 的combobox对象.
*/

QCtmComboBox::QCtmComboBox(QWidget* parent) : QComboBox(parent) { setView(new QListView(this)); }

/*!
    \brief      销毁该combobox对象.
*/
QCtmComboBox::~QCtmComboBox() {}

/*!
    \reimp
*/
void QCtmComboBox::showPopup()
{
    emit aboutToShowPopup();
    QComboBox::showPopup();
}

/*!
    \fn         void QCtmComboBox::aboutToShowPopup();
    \brief      弹出框显示前发送该信号.
*/
