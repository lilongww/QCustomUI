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

#include "QCtmMessage.h"

QCtmAbstractMessage::QCtmAbstractMessage()
{

}


struct QCtmMessage::Impl
{
	QString title;
	QString content;
	QDateTime time;
};

/**
 * @brief       构造函数
 * @param[in]   title 消息标题
 * @param[in] 	content 消息内容
 * @param[in]	time 消息时间
 */
QCtmMessage::QCtmMessage(const QString& title, const QString& content, const QDateTime& time)
	:m_impl(std::make_unique<Impl>())
{
	m_impl->title = title;
	m_impl->content = content;
	m_impl->time = time;
}

QCtmMessage::~QCtmMessage()
{
}

/**
 * @brief       设置消息标题
 * @param[in]   title 标题信息
 */
void QCtmMessage::setTitle(const QString& title)
{
	m_impl->title = title;
}

/**
 * @brief       获取标题信息
 * @Return:     标题
 */
const QString& QCtmMessage::title() const
{
	return m_impl->title;
}

/**
 * @brief       设置消息内容
 * @param[in]   content 消息内容
 */
void QCtmMessage::setContent(const QString& content)
{
	m_impl->content = content;
}

/**
 * @brief       获取消息内容
 * @Return:     消息内容
 */
const QString& QCtmMessage::content() const
{
	return m_impl->content;
}

/**
 * @brief       设置消息时间
 * @param[in]   time 消息时间
 */
void QCtmMessage::setDateTime(const QDateTime& time)
{
	m_impl->time = time;
}

/**
 * @brief       获取消息时间
 * @Return:     时间
 */
const QDateTime& QCtmMessage::dateTime() const
{
	return m_impl->time;
}
