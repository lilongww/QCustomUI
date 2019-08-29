/*!***************************************************************************
*  Copyright (C) 2019 lilong(DLC)
*  This file is part of the QCustomUi
*  Contact: 634916105@qq.com
*  License: LGPL
*****************************************************************************/
#pragma once

#include <QWidget>

class QCtmFlexWidget;

class QCtmDockHelper : public QWidget
{
	Q_OBJECT

public:
	QCtmDockHelper(QCtmFlexWidget *parent);
	~QCtmDockHelper();

private:
	void init();
};
