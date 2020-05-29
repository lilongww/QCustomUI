#include "QCtmLogFilterModel.h"
#include "QCtmLogModel.h"

struct QCtmLogFilterModel::Impl
{
    bool showLogs[QtMsgType::QtInfoMsg + 1];
    QString keywords;
};

QCtmLogFilterModel::QCtmLogFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_impl(std::make_unique<Impl>())
{
}

QCtmLogFilterModel::~QCtmLogFilterModel()
{
}

/**
 * @brief		查找日志中含有关键字的消息
 * @param[in]	keywords 关键字
 */
void QCtmLogFilterModel::search(const QString& keywords)
{
    m_impl->keywords = keywords;
    this->invalidate();
}

bool QCtmLogFilterModel::filterAcceptsRow(int sourceRow, [[maybe_unused]] const QModelIndex &sourceParent) const
{
    auto type = this->sourceModel()->data(this->sourceModel()->index(sourceRow, 0), QCtmLogModel::TypeRole).toInt();
    if (!m_impl->keywords.isEmpty())
    {
        const auto& desc = this->sourceModel()->data(this->sourceModel()->index(sourceRow, 1), Qt::DisplayRole).toString();
        return m_impl->showLogs[type] && desc.contains(m_impl->keywords, Qt::CaseInsensitive);
    }
    return m_impl->showLogs[type];
}

/**
 * @brief		设置消息类型是否显示
 * @param[in]	type 消息类型
 * @param[in]	show 是否显示
 */
void QCtmLogFilterModel::showLog(QtMsgType type, bool show)
{
    m_impl->showLogs[type] = show;
}
