/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/

#include "QCtmRecentModel.h"

struct QCtmRecentModel::Impl
{
    QString filter;
    Qt::CaseSensitivity cs;
    QVector<QCtmRecentData> d;
    QVector<QVector<std::reference_wrapper<QCtmRecentData>>> sorted { static_cast<int>(Classification::ClassificationSize) };

    inline void sortDatas()
    {
        for (auto& type : sorted)
            type.clear();
        auto now = QDateTime::currentDateTime();
        for (auto& data : d)
        {
            if (!filter.isEmpty())
            {
                if (!data.name.contains(filter, cs))
                    continue;
            }
            auto days = data.time.daysTo(now);
            if (days < 0)
                continue;
            if (data.isTop)
                sorted[Classification::Top].push_back(data);
            else if (now.date() == data.time.date())
                sorted[Classification::Today].push_back(data);
            else if (data.time.date().daysTo(now.date()) <= 1)
                sorted[Classification::Yesterday].push_back(data);
            else if (days <= 7)
                sorted[Classification::Pastweek].push_back(data);
            else if (days <= 30)
                sorted[Classification::Pastmonth].push_back(data);
            else
                sorted[Classification::Earlier].push_back(data);
        }
    }
};

/*!
    \class      QCtmRecentModel
    \brief      仿vs启动界面最近项目表model.
    \inherits   QAbstractItemModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmRecentModel.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmRecentModel::QCtmRecentModel(QObject* parent) : QAbstractItemModel(parent), m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
QCtmRecentModel::~QCtmRecentModel() {}

/*!
    \reimp
*/
int QCtmRecentModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (parent.isValid())
    {
        if (!parent.parent().isValid())
            return m_impl->sorted[parent.row()].size();
        return 0;
    }
    else
        return m_impl->sorted.size();
}

/*!
    \reimp
*/
int QCtmRecentModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const { return 1; }

/*!
    \reimp
*/
QVariant QCtmRecentModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return {};
    if (role == Qt::DisplayRole)
    {
        if (!index.parent().isValid())
        {
            switch (index.row())
            {
            case Classification::Top:
                return tr("Top");
            case Classification::Today:
                return tr("Today");
            case Classification::Yesterday:
                return tr("Yesterday");
            case Classification::Pastweek:
                return tr("This Week");
            case Classification::Pastmonth:
                return tr("This Month");
            case Classification::Earlier:
                return tr("Older");
            }
        }
    }
    else
    {
        if (index.parent().isValid())
        {
            const auto& data = m_impl->sorted[index.parent().row()][index.row()].get();
            switch (role)
            {
            case Roles::Name:
                return data.name;
            case Roles::Path:
                return data.path;
            case Roles::Icon:
                return data.icon;
            case Roles::Time:
                return data.time;
            case Roles::isTop:
                return data.isTop;
            }
        }
    }
    return {};
}

/*!
    \reimp
*/
QVariant QCtmRecentModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const { return {}; }

/*!
    \reimp
*/
bool QCtmRecentModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    if (index.parent().isValid())
    {
        auto parentRow = index.parent().row();
        auto row       = index.row();
        auto& data     = m_impl->sorted[parentRow][row].get();
        switch (role)
        {
        case Roles::isTop:
            {
                data.isTop = value.toBool();
                beginResetModel();
                m_impl->sortDatas();
                endResetModel();
                return true;
            }
        }
    }
    return false;
}

/*!
    \brief      设置最近使用的项目数据 \a datas, model会自动根据数据的时间和是否置顶来自动分类.
    \sa         recentDatas
*/
void QCtmRecentModel::setRecentDatas(const QVector<QCtmRecentData>& datas)
{
    beginResetModel();
    m_impl->d = datas;
    m_impl->sortDatas();
    endResetModel();
}

/*!
    \overload
                重载函数, 右值优化版本 \a datas.
    \sa         setRecentDatas, recentDatas
*/
void QCtmRecentModel::setRecentDatas(QVector<QCtmRecentData>&& datas)
{
    beginResetModel();
    m_impl->d = std::move(datas);
    m_impl->sortDatas();
    endResetModel();
}

/*!
    \reimp
*/
QModelIndex QCtmRecentModel::parent(const QModelIndex& child) const
{
    if (child.internalPointer())
    {
        auto it = std::find_if(
            m_impl->sorted.begin(), m_impl->sorted.end(), [&](const auto& vec) { return &vec == child.constInternalPointer(); });
        if (it != m_impl->sorted.end())
        {
            return createIndex(std::distance(m_impl->sorted.begin(), it), 0, nullptr);
        }
        return {};
    }
    return {};
}

/*!
    \reimp
*/
QModelIndex QCtmRecentModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return createIndex(row, column, nullptr);
    }
    else
    {
        return createIndex(row, column, &m_impl->sorted[parent.row()]);
    }
}

/*!
    \brief      返回最近使用的项目数据列表.
    \sa         setRecentDatas
*/
const QVector<QCtmRecentData>& QCtmRecentModel::recentDatas() const { return m_impl->d; }

/*!
    \brief      返回 \a index 对应的最近使用的项目.
*/
std::optional<QCtmRecentData> QCtmRecentModel::dataOfIndex(const QModelIndex& index) const
{
    if (!index.isValid() || !index.parent().isValid())
        return std::nullopt;
    return m_impl->sorted[index.parent().row()][index.row()].get();
}

/*!
    \brief      查找名称中包含 \a name 的项, 以及是否忽略大小写 \a cs.
*/
void QCtmRecentModel::search(const QString& name, Qt::CaseSensitivity cs)
{
    beginResetModel();
    m_impl->filter = name;
    m_impl->cs     = cs;
    m_impl->sortDatas();
    endResetModel();
}
