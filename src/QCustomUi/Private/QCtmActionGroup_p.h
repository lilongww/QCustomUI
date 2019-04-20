#pragma once

#include <QAction>

#include <memory>

class QCtmActionGroup : public QObject
{
    Q_OBJECT

public:
    QCtmActionGroup(QObject *parent);
    ~QCtmActionGroup();

    void addAction(QAction* action);
    void removeAction(QAction* action);
    private slots:
    void actionChanged();
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
