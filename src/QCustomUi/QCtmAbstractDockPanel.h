#pragma once

#include <QWidget>

class QCtmAbstractDockPanel : public QWidget
{
	Q_OBJECT

public:
	QCtmAbstractDockPanel(QWidget *parent);
	~QCtmAbstractDockPanel();
};
