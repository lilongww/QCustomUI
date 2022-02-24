#include "LoadingExample.h"

#include <QCustomUi/QCtmLoadingDialog.h>
#include <QCustomUi/QCtmMessageBox.h>

#include <QPushButton>
#include <QTimer>

#include <thread>

LoadingExample::LoadingExample(QWidget* parent) : QCtmWindow(parent)
{
    setWindowTitle(tr("LoadingExample"));
    auto btn = new QPushButton(this->centralWidget());
    btn->move(100, 100);
    btn->setText(tr("Show Loading 3 seconds"));
    auto loading = new QCtmLoadingDialog(this);
    loading->setCancelEnable(true);
    connect(btn,
            &QPushButton::clicked,
            this,
            [=]()
            {
                std::thread(
                    [&]
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                        QMetaObject::invokeMethod(loading, &QCtmLoadingDialog::done);
                    })
                    .detach();
                auto ret = loading->exec();
                if (ret == QCtmLoadingDialog::Result::Done)
                {
                    QCtmMessageBox::information(this, "Tips", "Loading finished.");
                }
                else
                {
                    QCtmMessageBox::information(this, "Tips", "Loading canceled.");
                }
            });
}
