#include "LoadingExample.h"

#include <QCustomUi/QCtmLoadingDialog.h>

#include <QPushButton>
#include <QTimer>

LoadingExample::LoadingExample(QWidget* parent)
    : QCtmWindow(parent)
{
    setWindowTitle(tr("LoadingExample"));
    auto btn = new QPushButton(this->centralWidget());
    btn->move(100, 100);
    btn->setText(tr("Show Loading 3 seconds"));
    auto loading = new QCtmLoadingDialog(this);
    connect(btn, &QPushButton::clicked, this, [=]()
        {
            QTimer::singleShot(std::chrono::milliseconds(3000), [=]() {loading->close(); });
            loading->exec();
        });
}
