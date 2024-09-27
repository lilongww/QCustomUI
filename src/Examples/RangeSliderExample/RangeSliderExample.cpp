#include "RangeSliderExample.h"

#include <QCustomUi/QCtmRangeSlider.h>

#include <QVBoxLayout>

RangeSliderExample::RangeSliderExample(QWidget* parent /*= nullptr*/) : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    auto slider = new QCtmRangeSlider(this);
    layout->addWidget(slider);
    slider->setValue(24, 75);
    slider->setTickPosition(QCtmRangeSlider::TickPosition::TicksBelow);
}

RangeSliderExample::~RangeSliderExample() {}
