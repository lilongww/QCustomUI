#pragma once

#include <QWidget>

#include <memory>

class QCtmWidgetItem : public QObject
{
public:
    QCtmWidgetItem(QAction* action, Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QCtmWidgetItem();

    void release();

    QAction* action() const;
    QWidget* widget() const;
    bool isCustomWidget() const;
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};

typedef std::shared_ptr<QCtmWidgetItem> QCtmWidgetItemPtr;
