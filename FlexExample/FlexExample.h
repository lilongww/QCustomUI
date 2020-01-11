#pragma once

#include <QtWidgets/QWidget>
#include "ui_FlexExample.h"

class FlexExample : public QWidget
{
	Q_OBJECT

public:
	FlexExample(QWidget *parent = Q_NULLPTR);

private:
	Ui::FlexExampleClass ui;
};
