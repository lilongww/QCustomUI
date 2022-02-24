#include "StageProgressBarExample.h"

#include <QCustomUi/QCtmStageProgressBar.h>

#include <QGridLayout>
#include <QSlider>

StageProgressBarExample::StageProgressBarExample(QWidget* parent) : QCtmWindow(parent) { init(); }

StageProgressBarExample::~StageProgressBarExample() {}

void StageProgressBarExample::init()
{
    auto layout       = new QGridLayout(this->centralWidget());
    auto progressBar1 = new QCtmStageProgressBar(this);
    int i             = 0;
    progressBar1->setStageCount(7);
    progressBar1->setText(i++, "Choose any one\n you like.");
    progressBar1->setText(i++,
                          "Two or two and a half lectures,\nwhere I am going to be talking about the \nbasic premises of this course.");
    progressBar1->setText(i++, "Three day's vacation should depressurize me.");
    progressBar1->setText(i++, "The department offers four specialities.");
    progressBar1->setText(i++, "We work from nine to five.");
    progressBar1->setText(i++, "He stands six feet seven.");
    progressBar1->setText(i++, "I wake at seven every morning.");
    // progressBar1->setTextVisible(true);
    progressBar1->setOrientation(Qt::Vertical);
    // progressBar1->setStageCricleRadius(20);
    layout->addWidget(progressBar1, 0, 0);

    auto progressBar2 = new QCtmStageProgressBar(this);
    i                 = 0;
    progressBar2->setStageCount(7);
    progressBar2->setText(i++, "one more word");
    progressBar2->setText(i++, "two");
    progressBar2->setText(i++, "three");
    progressBar2->setText(i++, "four");
    progressBar2->setText(i++, "five");
    progressBar2->setText(i++, "six");
    progressBar2->setText(i++, "seven");
    progressBar2->setTextVisible(true);
    layout->addWidget(progressBar2, 1, 0);

    auto slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    layout->addWidget(slider, 2, 0);
    connect(slider,
            &QSlider::valueChanged,
            this,
            [=](double value)
            {
                progressBar1->setValue(value);
                progressBar2->setValue(value);
            });
    slider->setValue(20);
}
