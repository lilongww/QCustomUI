#include "RangeSliderExample.h"

#include <QCustomUi/QCtmRangeSlider.h>

#include <QVBoxLayout>

RangeSliderExample::RangeSliderExample(QWidget* parent /*= nullptr*/) : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    {
        auto slider = new QCtmRangeSlider(this);
        layout->addWidget(slider);
        slider->setValue(24, 75);
        slider->setTickPosition(QCtmRangeSlider::TickPosition::TicksBelow);
    }

    {
        auto slider = new QCtmRangeSlider(this);
        layout->addWidget(slider);
        slider->setValue(24, 75);
        slider->setChunkVisible(true);
        slider->setStyleSheet("QCtmRangeSlider::chunk"
                              "{"
                              "    margin-top: 5px;"
                              "    margin-bottom: 5px;"
                              "    background: #4a737a;"
                              "    border-radius: 5px;"
                              "}"
                              "QCtmRangeSlider::groove:horizontal"
                              "{"
                              "    border: none;"
                              "    background: #ffffff;"
                              "    margin-top: 5px;"
                              "    margin-bottom: 5px;"
                              "    border-radius: 5px;"
                              "}"
                              "QCtmRangeSlider::handle:horizontal"
                              "{"
                              "    background: none;"
                              "}");
    }
}

RangeSliderExample::~RangeSliderExample() {}
