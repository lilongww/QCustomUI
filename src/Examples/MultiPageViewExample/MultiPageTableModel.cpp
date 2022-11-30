#include "MultiPageTableModel.h"

struct MultiPageTableModel::Impl
{
    QVector<Data> datas;
};

MultiPageTableModel::MultiPageTableModel(QObject* parent) : QCtmAbstractMultiPageTableModel(parent), m_impl(std::make_unique<Impl>()) {}

MultiPageTableModel::~MultiPageTableModel() {}

void MultiPageTableModel::setTestDatas(const QVector<Data>& datas)
{
    beginResetModel();
    m_impl->datas = datas;
    endResetModel();
}

int MultiPageTableModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
    return std::min<int>(m_impl->datas.size() - offset(), pageRowCount());
}

int MultiPageTableModel::columnCount(const QModelIndex& parent /* = QModelIndex() */) const { return 3; }

QVariant MultiPageTableModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    auto row = offset() + index.row();
    if (row < 0 || row >= m_impl->datas.size())
        return {};
    if (role == Qt::DisplayRole)
    {
        const auto& d = m_impl->datas[row];
        switch (index.column())
        {
        case 0:
            return d.col1;
        case 1:
            return d.col2;
        case 2:
            return d.col3;
        }
    }
    return {};
}

int MultiPageTableModel::pageCount() const
{
    return m_impl->datas.size() / pageRowCount() + static_cast<bool>(m_impl->datas.size() % pageRowCount());
}
