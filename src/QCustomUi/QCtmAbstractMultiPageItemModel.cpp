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

#include "QCtmAbstractMultiPageItemModel.h"

struct QCtmAbstractMultiPageItemModel::Impl
{
    int currentPage { 0 };
    int pageRowCount { 20 };
    int pageCountCache { 0 };
};

/*!
    \class      QCtmAbstractMultiPageItemModel
    \brief      分页 Model 基类.
    \inherits   QAbstractItemModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmAbstractMultiPageItemModel.h
*/

/*!
    \fn         virtual int QCtmAbstractMultiPageItemModel::pageCount() const;
    \brief      返回页面数量.
*/

/*!
    \fn         void QCtmAbstractMultiPageItemModel::currentPageChanged(int page);
    \brief      当前页面 \a page 改变时发送该信号.
    \sa         setCurrentPage, currentPage
*/

/*!
    \fn         void QCtmAbstractMultiPageItemModel::pageCountChanged(int count);
    \brief      页面数量 \a count 改变时发送该信号.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmAbstractMultiPageItemModel::QCtmAbstractMultiPageItemModel(QObject* parent /*= nullptr*/)
    : QAbstractItemModel(parent), m_impl(std::make_unique<Impl>())
{
}

/*!
    \brief      析构函数.
*/
QCtmAbstractMultiPageItemModel::~QCtmAbstractMultiPageItemModel() {}

/*!
    \brief      返回当前页面.
    \sa         setCurrentPage
*/
int QCtmAbstractMultiPageItemModel::currentPage() const { return m_impl->currentPage; }

/*!
    \brief      返回每页的行数量.
    \sa         setPageRowCount
*/
int QCtmAbstractMultiPageItemModel::pageRowCount() const { return m_impl->pageRowCount; }

/*!
    \brief      返回数据偏移量，即首行在所有数据中的位置.
*/
int QCtmAbstractMultiPageItemModel::offset() const { return m_impl->currentPage * m_impl->pageRowCount; }

/*!
    \brief      设置当前页 \a page.
    \sa         currentPage
*/
void QCtmAbstractMultiPageItemModel::setCurrentPage(int page)
{
    if (page < 0)
        return;
    if (page == m_impl->currentPage)
        return;
    m_impl->currentPage = page;
    emit dataChanged(index(0, 0), index(columnCount() - 1, rowCount() - 1));
    emit currentPageChanged(page);
}

/*!
    \brief      设置每页的行数量 \a rowCount.
    \sa         pageRowCount
*/
void QCtmAbstractMultiPageItemModel::setPageRowCount(int rowCount)
{
    beginResetModel();
    m_impl->pageRowCount = rowCount;
    auto pc              = pageCount();
    setCurrentPage(std::clamp(m_impl->currentPage, 0, pc));
    endResetModel();
    if (m_impl->pageCountCache != pc)
    {
        m_impl->pageCountCache = pc;
        emit pageCountChanged(m_impl->pageCountCache);
    }
}

/*!
    \brief      转到上一页.
    \sa         next
*/
void QCtmAbstractMultiPageItemModel::prev()
{
    if (m_impl->currentPage > 0)
        setCurrentPage(m_impl->currentPage - 1);
}

/*!
    \brief      转到下一页.
    \sa         prev
*/
void QCtmAbstractMultiPageItemModel::next()
{
    if (m_impl->currentPage + 1 < pageCount())
        setCurrentPage(m_impl->currentPage + 1);
}
