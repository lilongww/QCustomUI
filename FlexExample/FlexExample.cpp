#include "FlexExample.h"

#include <QCustomUi/QCtmDockTitle.h>

#include <QVBoxLayout>

FlexExample::FlexExample(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(new QCtmDockTitle(this));
}
