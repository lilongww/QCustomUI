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
