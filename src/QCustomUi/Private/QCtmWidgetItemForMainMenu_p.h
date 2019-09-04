#pragma once

#include <QWidget>

#include <memory>

class QCtmWidgetItemForMainMenu : public QObject
{
	Q_OBJECT
public:
    QCtmWidgetItemForMainMenu(QAction* action, Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QCtmWidgetItemForMainMenu();

    void release();

    QAction* action() const;
    QWidget* widget() const;
    bool isCustomWidget() const;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

typedef std::shared_ptr<QCtmWidgetItemForMainMenu> QCtmWidgetItemForMainMenuPtr;
