#include "InputExamples.h"

#include <QCustomUi/QCtmIPAddressEdit.h>
#include <QCustomUi/QCtmMultiComboBox.h>

#include <QGridLayout>
#include <QStandardItemModel>

InputExamples::InputExamples(QWidget *parent)
    : QCtmWindow(parent)
{
    auto layout = new QGridLayout(this->centralWidget());
    {
        QCtmIPAddressEdit* edit = new QCtmIPAddressEdit(this);
        edit->setIPAddress("127.0.0.1");
        layout->addWidget(edit, 0, 0);
    }
    {
        auto box = new QCtmMultiComboBox(this);
        QStringList items;
        for (int i = 0;i < 10; i++)
        {
            items.append(tr("item %1").arg(i));
        }
        box->addItems(items);
        layout->addWidget(box, 0, 1);
    }
    layout->setColumnStretch(1, 1);
}
