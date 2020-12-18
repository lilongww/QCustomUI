#include "QCtmDockTabWidget_p.h"

#include <QTabBar>

QCtmDockTabWidget::QCtmDockTabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    tabBar()->setVisible(false);
}

QCtmDockTabWidget::~QCtmDockTabWidget()
{
}

void QCtmDockTabWidget::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
    if (count() > 1)
        tabBar()->setVisible(true);
}

void QCtmDockTabWidget::tabRemoved(int index)
{
    QTabWidget::tabRemoved(index);
    if (count() < 2)
        tabBar()->setVisible(false);
}
