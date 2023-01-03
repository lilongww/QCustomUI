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

#include "QCtmAbstractMessageTipModel.h"

struct QCtmAbstractMessageTipModel::Impl
{
    QVector<QCtmAbstractMessageTipDataPtr> messages;
    int maxCount { 10000 };
    bool reversedOrder { true };
};

/*!
    \class      QCtmAbstractMessageTipModel
    \brief      消息model接口类，定义消息model的基本接口，要定义自己的消息model可以继承自该类.
    \inherits   QAbstractTableModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmAbstractMessageTipModel.h
*/

/*!
    \property   QCtmAbstractMessageTipModel::reversedOrder
    \brief      消息是否逆序显示，即最新一条消息显示在第一条，默认为真.
    \sa         setReversedOrder, reversedOrder
*/

/*!
    \brief      构造一个父对象为 \a parent 的消息对象.
*/
QCtmAbstractMessageTipModel::QCtmAbstractMessageTipModel(QObject* parent) : QAbstractTableModel(parent), m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      销毁当前消息model对象.
*/
QCtmAbstractMessageTipModel::~QCtmAbstractMessageTipModel() {}

/*!
    \brief      添加一条消息 \a msg.
    \sa         insertMessage, removeMessage
*/
void QCtmAbstractMessageTipModel::addMessage(QCtmAbstractMessageTipDataPtr msg) { insertMessage(m_impl->messages.size(), msg); }

/*!
    \brief      在 \a index 的位置插入一条消息对象 \a msg.
    \sa         addMessage, removeMessage
*/
void QCtmAbstractMessageTipModel::insertMessage(int index, QCtmAbstractMessageTipDataPtr msg)
{
    if (m_impl->maxCount <= 0)
        return;
    m_impl->messages.insert(index, msg);
    insertRow(m_impl->reversedOrder ? m_impl->messages.size() - index : index);
    while (m_impl->messages.size() >= m_impl->maxCount && !m_impl->messages.isEmpty())
    {
        removeMessage(m_impl->messages.front());
    }
}

/*!
    \brief      从消息model中移除消息对象 \a msg.
    \sa         addMessage, insertMessage
*/
void QCtmAbstractMessageTipModel::removeMessage(QCtmAbstractMessageTipDataPtr msg)
{
    const auto& index = m_impl->messages.indexOf(msg);
    removeRow(m_impl->reversedOrder ? m_impl->messages.size() - 1 - index : index);
    m_impl->messages.removeOne(msg);
}

/*!
    \brief      返回第 \a row 行的消息对象.
*/
QCtmAbstractMessageTipDataPtr QCtmAbstractMessageTipModel::message(int row) const
{
    return m_impl->messages.at(m_impl->reversedOrder ? m_impl->messages.size() - 1 - row : row);
}

/*!
    \brief      清除所有消息对象.
*/
void QCtmAbstractMessageTipModel::clear()
{
    beginResetModel();
    m_impl->messages.clear();
    endResetModel();
}

/*!
    \brief      设置消息最大保存数量 \a count.
    \sa         maximumCount()
*/
void QCtmAbstractMessageTipModel::setMaximumCount(int count) { m_impl->maxCount = count; }

/*!
    \brief      返回消息最大保存对象.
    \sa         setMaximumCount
*/
int QCtmAbstractMessageTipModel::maximumCount() const { return m_impl->maxCount; }

/*!
    \brief      设置消息是否逆序显示 \a re, 即最新一条显示在第一条，默认为真.
    \sa         reversedOrder
*/
void QCtmAbstractMessageTipModel::setReversedOrder(bool re)
{
    beginResetModel();
    m_impl->reversedOrder = re;
    endResetModel();
}

/*!
    \brief      消息是否逆序显示，即最新一条显示在第一条，默认为真.
    \sa         setReversedOrder
*/
bool QCtmAbstractMessageTipModel::reversedOrder() const { return m_impl->reversedOrder; }

/*!
    \reimp
*/
int QCtmAbstractMessageTipModel::rowCount([[maybe_unused]] const QModelIndex& parent /*= QModelIndex()*/) const
{
    return m_impl->messages.size();
}

/*!
    \reimp
*/
bool QCtmAbstractMessageTipModel::removeRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

/*!
    \reimp
*/
bool QCtmAbstractMessageTipModel::insertRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}
