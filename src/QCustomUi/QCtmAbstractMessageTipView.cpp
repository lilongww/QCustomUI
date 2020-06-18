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

#include "QCtmAbstractMessageTipView.h"
#include "QCtmAbstractMessageTipModel.h"

#include <QEvent>

/*!
    \class      QCtmAbstractMessageTipView
    \brief      QCtmAbstractMessageTipView provide abstract interface for message tip view.
    \inherits   QCtmNavigationSidePane
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a message tip view with \a parent.
*/
QCtmAbstractMessageTipView::QCtmAbstractMessageTipView(QCtmNavigationBar *parent)
	: QCtmNavigationSidePane(parent)
{
	this->setDockArea(QCtmNavigationSidePane::DockArea::Right);
}

/*!
    \brief      Destroys the message tip view.
*/
QCtmAbstractMessageTipView::~QCtmAbstractMessageTipView()
{
}

/*!
    \fn         void QCtmAbstractMessageTipView::setModel(QCtmAbstractMessageTipModel* model)
    \brief      Sets the message tip \a model.
    \sa         model()
*/

/*!
    \fn         QCtmAbstractMessageTipModel* QCtmAbstractMessageTipView::model() const 
    \brief      Returns the message tip model.
    \sa         setModel
*/
