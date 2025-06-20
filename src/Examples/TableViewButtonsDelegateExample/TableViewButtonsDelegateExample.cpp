#include "TableViewButtonsDelegateExample.h"

#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmTableViewButtonsDelegate.h>

#include <QStandardItemModel>

TableViewButtonsDelegateExample::TableViewButtonsDelegateExample(QWidget* parent /*= nullptr*/) : QCtmWindow(parent)
{
    ui.setupUi(centralWidget());
    auto model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setRowCount(10);
    for (int i = 0; i < model->rowCount(); ++i)
    {
        for (int j = 0; j < model->columnCount() - 1; ++j)
        {
            auto item = new QStandardItem(QString("Item %1,%2").arg(i).arg(j));
            item->setEditable(false);
            model->setItem(i, j, item);
        }
    }
    auto fn = [this](int btn, const QModelIndex& index)
    {
        QCtmMessageBox::information(
            this, "Tips", QString("index(%1, %2) button %3 clicked").arg(index.row()).arg(index.column()).arg(btn + 1));
    };
    auto d1 = new QCtmTableViewButtonsDelegate(ui.tableView);
    ui.tableView->setModel(model);
    ui.tableView->setItemDelegateForColumn(2, d1);
    d1->addButton(QLatin1String("Button 1"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d1->addButton(QLatin1String("Button 2"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d1->addButton(QLatin1String("Button 3"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d1->setAlignment(Qt::AlignLeft);
    connect(d1, &QCtmTableViewButtonsDelegate::buttonClicked, this, fn);

    auto d2 = new QCtmTableViewButtonsDelegate(ui.tableView_2);
    ui.tableView_2->setModel(model);
    ui.tableView_2->setItemDelegateForColumn(2, d2);
    d2->addButton(QLatin1String("Button 1"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d2->addButton(QLatin1String("Button 2"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d2->addButton(QLatin1String("Button 3"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d2->setAlignment(Qt::AlignRight);
    connect(d2, &QCtmTableViewButtonsDelegate::buttonClicked, this, fn);

    auto d3 = new QCtmTableViewButtonsDelegate(ui.tableView_3);
    ui.tableView_3->setModel(model);
    ui.tableView_3->setItemDelegateForColumn(2, d3);
    d3->addButton(QLatin1String("Button 1"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d3->addButton(QLatin1String("Button 2"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d3->addButton(QLatin1String("Button 3"), QBrush(Qt::lightGray), QBrush(Qt::darkGray), QBrush(Qt::red), QColor(Qt::black));
    d3->setAlignment(Qt::AlignCenter);
    connect(d3, &QCtmTableViewButtonsDelegate::buttonClicked, this, fn);
}

TableViewButtonsDelegateExample::~TableViewButtonsDelegateExample()
{
}
