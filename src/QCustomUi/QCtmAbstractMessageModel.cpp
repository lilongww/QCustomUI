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

#include "QCtmAbstractMessageModel.h"

struct QCtmAbstractMessageModel::Impl
{
	QList<QCtmAbstractMessagePtr> messages;
	int maxCount{ 10000 };
};

QCtmAbstractMessageModel::QCtmAbstractMessageModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_impl(std::make_unique<Impl>())
{
}

QCtmAbstractMessageModel::~QCtmAbstractMessageModel()
{
}

/**
 * @brief  添加一条消息
 * @param  msg 消息内容
 */
void QCtmAbstractMessageModel::addMessage(QCtmAbstractMessagePtr msg)
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

/**
 * @brief  插入一条消息
 * @param  index 要插入的位置
 * @param  msg 消息内容
 */
void QCtmAbstractMessageModel::insertMessage(int index, QCtmAbstractMessagePtr msg)
{
	m_impl->messages.insert(index, msg);
	insertRow(index);
}

/**
 * @brief  移除一条消息
 * @param  msg 消息内容
 */
void QCtmAbstractMessageModel::removeMessage(QCtmAbstractMessagePtr msg)
{
	const auto& index = m_impl->messages.indexOf(msg);
	removeRow(index);
	m_impl->messages.removeOne(msg);
}

/**
 * @brief  获取指定行对应的消息内容
 * @param  row 指定行
 * @return 指定行对应的消息内容
 */
QCtmAbstractMessagePtr QCtmAbstractMessageModel::message(int row) const
{
	return m_impl->messages.at(row);
}

/**
 * @brief    清除所有消息
 */
void QCtmAbstractMessageModel::clear()
{
	beginResetModel();
	m_impl->messages.clear();
	endResetModel();
}

/**
 * @brief		设置最大保存消息数量，默认1万条
 * @param[in]	count 消息数量
 */
void QCtmAbstractMessageModel::setMaximumCount(int count)
{
	m_impl->maxCount = count;
}

/**
 * @brief		最大保存消息数量
 */
int QCtmAbstractMessageModel::maximumCount() const
{
	return m_impl->maxCount;
}

int QCtmAbstractMessageModel::rowCount([[maybe_unused]] const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_impl->messages.size();
}

bool QCtmAbstractMessageModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
	beginRemoveRows(parent, row, row + count - 1);
	endRemoveRows();
	return true;
}

bool QCtmAbstractMessageModel::insertRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
	beginInsertRows(parent, row, row + count - 1);
	endInsertRows();
	return true;
}
