/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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
    \brief      QCtmComboBox provide a combobox, the pop list view can be set style sheet.
    \inherits   QComboBox
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \sa         QComboBox
*/

/*!
    \brief      Constructs a combobox with given \a parent.
*/

QCtmComboBox::QCtmComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setView(new QListView(this));
}

/*!
    \brief      Destroys the combobox.
*/
QCtmComboBox::~QCtmComboBox()
{
}
