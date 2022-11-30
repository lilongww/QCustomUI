#include "MultiPageViewExample.h"

MultiPageViewExample::MultiPageViewExample()
{
    ui.setupUi(this->centralWidget());
    auto listModel = new QCtmMultiPageStringListModel(this);
    QStringList list;
    for (int i = 0; i < 999; ++i)
    {
        list.push_back(QString("Item %1").arg(i));
    }
    listModel->setStringList(std::move(list));
    ui.listView->setModel(listModel);
    ui.btnBox->setModel(listModel);
    listModel->setPageRowCount(50);
}

MultiPageViewExample::~MultiPageViewExample() {}
