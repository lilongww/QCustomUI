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

#include "QCtmAbstractMessageTipView.h"
#include "QCtmAbstractMessageTipModel.h"

#include <QEvent>

/*!
    \class      QCtmAbstractMessageTipView
    \brief      消息提示视图接口类.
    \inherits   QCtmNavigationSidePane
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmAbstractMessageTipView.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmAbstractMessageTipView::QCtmAbstractMessageTipView(QCtmNavigationBar* parent) : QCtmNavigationSidePane(parent)
{
    this->setDockArea(QCtmNavigationSidePane::DockArea::Right);
}

/*!
    \brief      析构函数.
*/
QCtmAbstractMessageTipView::~QCtmAbstractMessageTipView() {}

/*!
    \fn         void QCtmAbstractMessageTipView::setModel(QCtmAbstractMessageTipModel* model)
    \brief      设置消息提示 \a model.
    \sa         model()
*/

/*!
    \fn         QCtmAbstractMessageTipModel* QCtmAbstractMessageTipView::model() const
    \brief      返回消息提示 model.
    \sa         setModel
*/
