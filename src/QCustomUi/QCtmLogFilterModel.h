#pragma once

#include <QSortFilterProxyModel>

#include <memory>

class QCtmLogFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QCtmLogFilterModel(QObject *parent);
    ~QCtmLogFilterModel();

    /**
     * @brief		查找日志中含有关键字的消息
     * @param[in]	keywords 关键字
     */
    void search(const QString& keywords);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    public slots:
    /**
     * @brief		设置消息类型是否显示
     * @param[in]	type 消息类型
     * @param[in]	show 是否显示
     */
    void showLog(QtMsgType type, bool show);

private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
