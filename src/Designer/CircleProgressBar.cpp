#include "CircleProgressBar.h"

#include <QCustomUi/QCtmCircleProgressBar.h>

CircleProgressBar::CircleProgressBar(QObject* parent /*= nullptr*/) : QObject(parent = nullptr) {}

QString CircleProgressBar::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"QCtmCircleProgressBar\" name=\"circleProgressBar\">\n"
           " </widget>\n"
           "</ui>\n";
}

QWidget* CircleProgressBar::createWidget(QWidget* parent) { return new QCtmCircleProgressBar(parent); }
