#include "SelectionButtonBoxExample.h"

#include <QCustomUi/QCtmSelectionButtonBox.h>

#include <QTimer>
#include <QVBoxLayout>

SelectionButtonBoxExample::SelectionButtonBoxExample(QWidget* parent) : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    {
        auto box = new QCtmSelectionButtonBox({ "Test1", "Test2", "Test3" });
        layout->addWidget(box);
    }
    {
        auto box = new QCtmSelectionButtonBox();
        layout->addWidget(box);
        box->setTexts({ { "Test1", "Test2", "Test3" } });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::Exclusive);
    }
    {
        auto box = new QCtmSelectionButtonBox();
        layout->addWidget(box);
        box->setTexts({ { "Test1", "Test2", "Test3" } });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::ExclusiveOptional);
    }
    {
        auto box = new QCtmSelectionButtonBox(Qt::Vertical);
        layout->addWidget(box);
        box->setTexts({ { "Test1", "Test2", "Test3" } });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::ExclusiveOptional);
    }
}

SelectionButtonBoxExample::~SelectionButtonBoxExample() {}
