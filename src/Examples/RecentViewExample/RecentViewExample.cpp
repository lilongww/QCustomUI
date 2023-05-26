#include "RecentViewExample.h"

#include <QCustomUi/QCtmRecentModel.h>
#include <QCustomUi/QCtmRecentView.h>

#include <QHBoxLayout>
#include <QSlider>

RecentViewExample::RecentViewExample(QWidget* parent) : QCtmWindow(parent) { init(); }

RecentViewExample::~RecentViewExample() {}

void RecentViewExample::init()
{
    auto view  = new QCtmRecentView(this);
    auto model = new QCtmRecentModel(this);
    view->setModel(model);
    setCentralWidget(view);
    model->setRecentDatas(
        { { "File", "C:/File.txt", QDateTime::currentDateTime(), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File1", "C:/File1.txt", QDateTime::currentDateTime().addDays(-1), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File2", "C:/File2.txt", QDateTime::currentDateTime().addDays(-2), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File3", "C:/File3.txt", QDateTime::currentDateTime().addDays(-3), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File4", "C:/File4.txt", QDateTime::currentDateTime().addDays(-4), this->style()->standardIcon(QStyle::SP_FileIcon), true },
          { "File5", "C:/File5.txt", QDateTime::currentDateTime().addDays(-5), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File6", "C:/File6.txt", QDateTime::currentDateTime().addDays(-6), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File7", "C:/File7.txt", QDateTime::currentDateTime().addDays(-7), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File8", "C:/File8.txt", QDateTime::currentDateTime().addDays(-8), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File9", "C:/File9.txt", QDateTime::currentDateTime().addDays(-9), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File10", "C:/File10.txt", QDateTime::currentDateTime().addDays(-31), this->style()->standardIcon(QStyle::SP_FileIcon), false },
          { "File11",
            "C:/File11.txt",
            QDateTime::currentDateTime().addDays(-32),
            this->style()->standardIcon(QStyle::SP_FileIcon),
            false } });
    view->expandAll();
    // view->setTopButtonVisible(false);
    // QIcon icon(":/QCustomUi/Resources/tree-open-light.png");
    // icon.addPixmap(QPixmap(":/QCustomUi/Resources/tree-close-light.png"), QIcon::Mode::Normal, QIcon::On);
    // view->setTopButtonIcon(icon);
}
