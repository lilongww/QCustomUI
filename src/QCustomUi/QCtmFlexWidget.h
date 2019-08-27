/*!***************************************************************************
*  Copyright (C) 2019 lilong(DLC)
*  This file is part of the QCustomUi
*  Contact: 634916105@qq.com
*  License: LGPL
*****************************************************************************/

#pragma once

#include "QCtmAbstractDockPanel.h"

#include <memory>

class QCtmFlexWidget : public QCtmAbstractDockPanel
{
	Q_OBJECT

public:
	QCtmFlexWidget(QWidget *parent = nullptr);
	QCtmFlexWidget(Qt::Orientation orientation, QWidget* parent = nullptr);
	~QCtmFlexWidget();

	void setOrientation(Qt::Orientation orientation);
	Qt::Orientation orientation() const;

	void addDockPanel(QCtmAbstractDockPanel* panel);
	void removeDockPanel(QCtmAbstractDockPanel* panel);
	void insertDockPanel(int index, QCtmAbstractDockPanel* panel);
	int indexOf(QCtmAbstractDockPanel* panel) const;
	QCtmAbstractDockPanel* panel() const;
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
