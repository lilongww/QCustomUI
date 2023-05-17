#include "RecentViewExample.h"

#include <QCustomUi/QCtmRecentModel.h>
#include <QCustomUi/QCtmRecentView.h>

#include <QHBoxLayout>
#include <QSlider>

RecentViewExample::RecentViewExample(QWidget* parent) : QCtmWindow(parent) { init(); }

RecentViewExample::~RecentViewExample() {}

void RecentViewExample::init()
{
    auto view  = new QCtmRecentView(this);
    auto model = new QCtmRecentModel(this);
    view->setModel(model);
}
