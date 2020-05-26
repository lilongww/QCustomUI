#include "DisplayWidgetExamples.h"

#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmEditLabel.h>

#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

DisplayWidgetExamples::DisplayWidgetExamples(QWidget *parent)
    : QCtmWindow(parent)
{
	int row = 0;
    auto layout = new QGridLayout(this->centralWidget());
	{
		auto btn = new QPushButton(tr("Information Message Box"));
		layout->addWidget(btn, row, 0);
		connect(btn, &QPushButton::clicked, this, [=] {QCtmMessageBox::information(this, tr("Information"), tr("This is information message box")); });
	}
	{
		auto btn = new QPushButton(tr("Warning Message Box"));
		layout->addWidget(btn, row, 1);
		connect(btn, &QPushButton::clicked, this, [=] {QCtmMessageBox::warning(this, tr("Warning"), tr("This is warning message box")); });
	}
	{
		auto btn = new QPushButton(tr("Critical Message Box"));
		layout->addWidget(btn, row, 2);
		connect(btn, &QPushButton::clicked, this, [=] {QCtmMessageBox::critical(this, tr("Critical"), tr("This is critical message box")); });
	}
	{
		auto btn = new QPushButton(tr("Question Message Box"));
		layout->addWidget(btn, row++, 3);
		connect(btn, &QPushButton::clicked, this, [=] {QCtmMessageBox::question(this, tr("Question"), tr("This is question message box")); });
	}
	{
		auto label = new QCtmEditLabel(this);
		label->setText("QCtmEditLabel Display Text");
		layout->addWidget(label, row++, 0);
	}
	layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), row, 0);
	layout->setRowStretch(row, 1);
}
