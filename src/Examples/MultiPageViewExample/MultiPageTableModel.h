#pragma once

#include <QCustomUi/QCtmAbstractMultiPageTableModel.h>

#include <QVector>

struct Data
{
    QString col1;
    int col2;
    int col3;
};

class MultiPageTableModel : public QCtmAbstractMultiPageTableModel
{
    Q_OBJECT
public:
    MultiPageTableModel(QObject* parent = nullptr);
    ~MultiPageTableModel();

    void setTestDatas(const QVector<Data>& datas);
    int rowCount(const QModelIndex& parent /* = QModelIndex() */) const override;
    int columnCount(const QModelIndex& parent /* = QModelIndex() */) const override;
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;
    int pageCount() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};