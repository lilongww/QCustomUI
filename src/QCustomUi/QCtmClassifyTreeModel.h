#pragma once

#include "qcustomui_global.h"

#include <QAbstractItemModel>

#include <memory>

class QCtmClassifyTreeItem;

class QCUSTOMUI_EXPORT QCtmClassifyTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QCtmClassifyTreeModel(QObject* parent);
    ~QCtmClassifyTreeModel();

    void addItem(QCtmClassifyTreeItem* item);
    void insertItem(int index, QCtmClassifyTreeItem* item);
    void removeItem(QCtmClassifyTreeItem* item);
    void reset();
    QModelIndex indexFromItem(const QCtmClassifyTreeItem* item) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parnet = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
