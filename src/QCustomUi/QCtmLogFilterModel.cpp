/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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

#include "QCtmLogFilterModel.h"
#include "QCtmLogModel.h"

struct QCtmLogFilterModel::Impl
{
    bool showLogs[QtMsgType::QtInfoMsg + 1];
    QString keyword;
};

/*!
    \class      QCtmLogFilterModel
    \brief      日志筛选 model.
    \inherits   QSortFilterProxyModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLogFilterModel.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmLogFilterModel::QCtmLogFilterModel(QObject* parent) : QSortFilterProxyModel(parent), m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
QCtmLogFilterModel::~QCtmLogFilterModel() {}

/*!
    \brief      查询关键字 \a keyword.
*/
void QCtmLogFilterModel::search(const QString& keyword)
{
    m_impl->keyword = keyword;
    this->invalidate();
}

/*!
    \reimp
*/
bool QCtmLogFilterModel::filterAcceptsRow(int sourceRow, [[maybe_unused]] const QModelIndex& sourceParent) const
{
    auto type = this->sourceModel()->data(this->sourceModel()->index(sourceRow, 0), QCtmLogModel::TypeRole).toInt();
    if (!m_impl->keyword.isEmpty())
    {
        const auto& desc = this->sourceModel()->data(this->sourceModel()->index(sourceRow, 1), Qt::DisplayRole).toString();
        return m_impl->showLogs[type] && desc.contains(m_impl->keyword, Qt::CaseInsensitive);
    }
    return m_impl->showLogs[type];
}

/*!
    \brief      设置日志类型 \a type 是否显示 \a show.
*/
void QCtmLogFilterModel::showLog(QtMsgType type, bool show) { m_impl->showLogs[type] = show; }
