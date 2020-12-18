#pragma once

#include <QTabWidget>

class QCtmDockTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCtmDockTabWidget(QWidget* parent);
    ~QCtmDockTabWidget();

protected:
    void tabInserted(int index) override;
    void tabRemoved(int index) override;
};
