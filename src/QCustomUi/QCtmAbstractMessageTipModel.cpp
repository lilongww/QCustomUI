/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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
    QList<QCtmAbstractMessageTipDataPtr> messages;
    int maxCount{ 10000 };
};

/*!
    \class      QCtmAbstractMessageTipModel
    \brief      QCtmAbstractMessageTipModel provide abstract interface of message tip model.
    \inherits   QAbstractTableModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a message tip model with \a parent.
*/
QCtmAbstractMessageTipModel::QCtmAbstractMessageTipModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_impl(std::make_unique<Impl>())
{
}

/*!
    \brief      Destroys the model.
*/
QCtmAbstractMessageTipModel::~QCtmAbstractMessageTipModel()
{
}

/*!
    \brief      Add a message tip, \a msg.
    \sa         insertMessage, removeMessage
*/
void QCtmAbstractMessageTipModel::addMessage(QCtmAbstractMessageTipDataPtr msg)
{
    while (m_impl->messages.size() >= m_impl->maxCount && !m_impl->messages.isEmpty())
    {
        removeMessage(m_impl->messages.front());
    }

    if (m_impl->maxCount <= 0)
        return;

    m_impl->messages.push_back(msg);
    insertRow(rowCount());
}

/*!
    \brief      Insert a message tip with \a index and \a msg.
    \sa         addMessage, removeMessage
*/
void QCtmAbstractMessageTipModel::insertMessage(int index, QCtmAbstractMessageTipDataPtr msg)
{
    m_impl->messages.insert(index, msg);
    insertRow(index);
}

/*!
    \brief      Remove a message tip, \a msg.
    \sa         addMessage, insertMessage
*/
void QCtmAbstractMessageTipModel::removeMessage(QCtmAbstractMessageTipDataPtr msg)
{
    const auto& index = m_impl->messages.indexOf(msg);
    removeRow(index);
    m_impl->messages.removeOne(msg);
}

/*!
    \brief      Return the message tip by \a row.
*/
QCtmAbstractMessageTipDataPtr QCtmAbstractMessageTipModel::message(int row) const
{
    return m_impl->messages.at(row);
}

/*!
    \brief      Remove all message tips.
*/
void QCtmAbstractMessageTipModel::clear()
{
    beginResetModel();
    m_impl->messages.clear();
    endResetModel();
}

/*!
    \brief      Sets the maximum \a count of message tips.
    \sa         maximumCount()
*/
void QCtmAbstractMessageTipModel::setMaximumCount(int count)
{
    m_impl->maxCount = count;
}

/*!
    \brief      Returns the maximum count of message tips.
    \sa         setMaximumCount
*/
int QCtmAbstractMessageTipModel::maximumCount() const
{
    return m_impl->maxCount;
}

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
