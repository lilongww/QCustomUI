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

#pragma once

#include "qcustomui_global.h"
#include "QCtmNavigationSidePanel.h"

class QCtmAbstractMessageModel;
class QCtmNavigationBar;

class QCUSTOMUI_EXPORT QCtmAbstractMessageView : public QCtmNavigationSidePanel
{
	Q_OBJECT

public:
	QCtmAbstractMessageView(QCtmNavigationBar *parent);
	~QCtmAbstractMessageView();

	 /** 
	  * @brief  设置model
	  * @param  model 消息model	
	  */
	virtual void setModel(QCtmAbstractMessageModel* model) = 0;

	 /** 
	  * @brief  获取消息model
	  * @return 消息model
	  */
	virtual QCtmAbstractMessageModel* model()const = 0;

};
