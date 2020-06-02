#include "DisplayWidgetExamples.h"

#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmEditLabel.h>
#include <QCustomUi/QCtmStageProgressBar.h>

#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QSlider>

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
	{
		auto progressBar1 = new QCtmStageProgressBar(this);
		int i = 0;
		progressBar1->setStageCount(7);
		progressBar1->setText(i++, "Choose any one\n you like.");
		progressBar1->setText(i++, "Two or two and a half lectures,\nwhere I am going to be talking about the \nbasic premises of this course.");
		progressBar1->setText(i++, "Three day's vacation should depressurize me.");
		progressBar1->setText(i++, "The department offers four specialities.");
		progressBar1->setText(i++, "We work from nine to five.");
		progressBar1->setText(i++, "He stands six feet seven.");
		progressBar1->setText(i++, "I wake at seven every morning.");
		progressBar1->setTextVisible(true);
		progressBar1->setOrientation(Qt::Vertical);
		//progressBar1->setStageCricleRadius(20);
		layout->addWidget(progressBar1, row++, 0);

		auto progressBar2 = new QCtmStageProgressBar(this);
		i = 0;
		progressBar2->setStageCount(7);
		progressBar2->setText(i++, "one");
		progressBar2->setText(i++, "two");
		progressBar2->setText(i++, "three");
		progressBar2->setText(i++, "four");
		progressBar2->setText(i++, "five");
		progressBar2->setText(i++, "six");
		progressBar2->setText(i++, "seven");
		progressBar2->setTextVisible(true);
		layout->addWidget(progressBar2, row, 1);

		auto slider = new QSlider(this);
		slider->setOrientation(Qt::Horizontal);
		layout->addWidget(slider, row++, 0);
		connect(slider, &QSlider::valueChanged, this, [=](double value) {progressBar1->setValue(value); progressBar2->setValue(value); });
		slider->setValue(20);
	}
	layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), row, 0);
	layout->setRowStretch(row, 1);
}
