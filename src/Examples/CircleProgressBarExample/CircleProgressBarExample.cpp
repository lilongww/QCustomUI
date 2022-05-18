#include "CircleProgressBarExample.h"

#include <QCustomUi/QCtmCircleProgressBar.h>

#include <QGridLayout>
#include <QPushButton>
#include <QSlider>

CircleProgressBarExample::CircleProgressBarExample(QWidget* parent) : QCtmWindow(parent) { init(); }

CircleProgressBarExample::~CircleProgressBarExample() {}

void CircleProgressBarExample::init()
{
    auto layout       = new QVBoxLayout(this->centralWidget());
    auto progressBar1 = new QCtmCircleProgressBar(this);

    layout->addWidget(progressBar1, 1);

    auto slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    layout->addWidget(slider);
    connect(slider, &QSlider::valueChanged, this, [=](double value) { progressBar1->setValue(value); });
    slider->setValue(20);
    slider->setRange(0, 100);
    auto btn = new QPushButton("test", this);
    btn->setCheckable(true);
    layout->addWidget(btn);
    // progressBar1->setPenCapStyle(Qt::PenCapStyle::RoundCap);
    //  progressBar1->setBarWidth(30);
    connect(btn,
            &QPushButton::clicked,
            this,
            [=](bool checked)
            {
                if (checked)
                {
                    progressBar1->setRange(0, 0);
                }
                else
                {
                    progressBar1->setRange(0, 100);
                }
            });
}
