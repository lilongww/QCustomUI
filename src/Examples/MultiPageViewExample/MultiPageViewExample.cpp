#include "MultiPageViewExample.h"
#include "MultiPageTableModel.h"

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
    ui.btnBox->setPageButtonCount(0);

    auto tableModel = new MultiPageTableModel(this);
    ui.tableView->setModel(tableModel);
    ui.btnBox_2->setModel(tableModel);
    QVector<Data> datas;
    for (int i = 0; i < 999; ++i)
    {
        datas.push_back(Data { QString("col %1").arg(i), i, i });
    }
    tableModel->setTestDatas(datas);
}

MultiPageViewExample::~MultiPageViewExample() {}
