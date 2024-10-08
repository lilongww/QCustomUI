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
        slider->setTracking(false);
        slider->setTickPosition(QCtmRangeSlider::TickPosition::TicksBelow);
        connect(slider, &QCtmRangeSlider::valueChanged, this, [](int l, int u) { qDebug() << l << "-" << u; });
    }

    {
        auto slider = new QCtmRangeSlider(this);
        layout->addWidget(slider);
        slider->setValue(24, 75);
        slider->setChunkVisible(true);
        connect(slider, &QCtmRangeSlider::valueChanged, this, [](int l, int u) { qDebug() << l << "-" << u; });
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

    {
        auto slider = new QCtmRangeSlider(this);
        slider->setOrientation(Qt::Vertical);
        layout->addWidget(slider);
        slider->setValue(24, 75);
        connect(slider, &QCtmRangeSlider::valueChanged, this, [](int l, int u) { qDebug() << l << "-" << u; });
        slider->setTickPosition(QCtmRangeSlider::TickPosition::TicksBelow);
    }

    {
        auto slider = new QCtmRangeSlider(this);
        slider->setOrientation(Qt::Vertical);
        layout->addWidget(slider);
        slider->setValue(24, 75);
        slider->setChunkVisible(true);
        connect(slider, &QCtmRangeSlider::valueChanged, this, [](int l, int u) { qDebug() << l << "-" << u; });
        slider->setStyleSheet("QCtmRangeSlider::chunk"
                              "{"
                              "    margin-left: 5px;"
                              "    margin-right: 5px;"
                              "    background: #4a737a;"
                              "    border-radius: 5px;"
                              "}"
                              "QCtmRangeSlider::groove:vertical"
                              "{"
                              "    border: none;"
                              "    background: #ffffff;"
                              "    margin-left: 5px;"
                              "    margin-right: 5px;"
                              "    border-radius: 5px;"
                              "}"
                              "QCtmRangeSlider::handle:vertical"
                              "{"
                              "    background: none;"
                              "}");
    }
}

RangeSliderExample::~RangeSliderExample() {}
