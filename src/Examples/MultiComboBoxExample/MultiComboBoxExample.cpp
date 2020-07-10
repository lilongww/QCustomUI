#include "MultiComboBoxExample.h"

#include <QCustomUi/QCtmMultiComboBox.h>

#include <QHBoxLayout>

MultiComboBoxExample::MultiComboBoxExample(QWidget* parent)
    : QCtmWindow(parent)
{
    init();
    setWindowTitle(tr("MultiComboBox Example"));
}

MultiComboBoxExample::~MultiComboBoxExample()
{

}

void MultiComboBoxExample::init()
{
    auto box = new QCtmMultiComboBox(this);
    QStringList items{ tr("Item1"), tr("Item2"), tr("Item3"), tr("Item4"), tr("Item5") };
    box->addItems(items);

    auto layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    box->setMinimumWidth(200);
    layout->addWidget(box);
    layout->addStretch(1);
}
