#include "SelectionButtonBoxExample.h"

#include <QCustomUi/QCtmSelectionButtonBox.h>

#include <QVBoxLayout>

SelectionButtonBoxExample::SelectionButtonBoxExample(QWidget* parent) : QCtmWindow(parent)
{
    auto layout = new QVBoxLayout(this->centralWidget());
    {
        auto box = new QCtmSelectionButtonBox();
        layout->addWidget(box);
        box->setTexts({ u8"选项1", u8"选项2", u8"选项3" });
    }
    {
        auto box = new QCtmSelectionButtonBox();
        layout->addWidget(box);
        box->setTexts({ u8"选项1", u8"选项2", u8"选项3" });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::Exclusive);
    }
    {
        auto box = new QCtmSelectionButtonBox();
        layout->addWidget(box);
        box->setTexts({ u8"选项1", u8"选项2", u8"选项3" });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::ExclusiveOptional);
    }
    {
        auto box = new QCtmSelectionButtonBox(Qt::Vertical);
        layout->addWidget(box);
        box->setTexts({ u8"选项1", u8"选项2", u8"选项3" });
        box->setExclusionPolicy(QCtmSelectionButtonBox::ExclusionPolicy::ExclusiveOptional);
    }
}

SelectionButtonBoxExample::~SelectionButtonBoxExample() {}
