#include "StyleWidget.h"

#include <QCustomUi/QCustomUi.h>

#include <QStandardItemModel>

StyleWidget::StyleWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.checkBox_4->setCheckState(Qt::CheckState::PartiallyChecked);
    ui.checkBox_5->setCheckState(Qt::CheckState::PartiallyChecked);

    auto model = new QStandardItemModel(this);
    ui.tableView->setModel(model);
	ui.widget_4->setReadOnly(true);
	ui.widget_4->setIPAddress("192.168.1.1");
    model->setHorizontalHeaderLabels(QStringList() << "Test" << "Test" << "Test" << "Test" << "Test" << "Test");
    model->setRowCount(10);
    for (int i = 0;i<10;i++)
    {
        for (int j = 0;j<model->rowCount();j++)
        {
            model->setItem(i, j, new QStandardItem("Test Item"));
        }
    }
	ui.tabWidget->setAttribute(Qt::WA_StyledBackground);
	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		QCtmDialog dlg(this);
        //Dialog dlg(this);
		dlg.exec();
		});
}

StyleWidget::~StyleWidget()
{
}
