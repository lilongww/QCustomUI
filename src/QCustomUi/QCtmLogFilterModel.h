#pragma once

#include <QSortFilterProxyModel>

#include <memory>

class QCtmLogFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QCtmLogFilterModel(QObject *parent);
    ~QCtmLogFilterModel();

    void search(const QString& keywords);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
public slots:
    void showLog(QtMsgType type, bool show);
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
