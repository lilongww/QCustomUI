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
 * @brief       Set the message title.
 */
void QCtmMessage::setTitle(const QString& title)
{
	m_impl->title = title;
}

/**
 * @brief       Get the message title.
 */
const QString& QCtmMessage::title() const
{
	return m_impl->title;
}

/**
 * @brief       Set the message content.
 */
void QCtmMessage::setContent(const QString& content)
{
	m_impl->content = content;
}

/**
 * @brief       Get the message content.
 */
const QString& QCtmMessage::content() const
{
	return m_impl->content;
}

/**
 * @brief       Set the message date time.
 */
void QCtmMessage::setDateTime(const QDateTime& time)
{
	m_impl->time = time;
}

/**
 * @brief       Get the message date time.
 */
const QDateTime& QCtmMessage::dateTime() const
{
	return m_impl->time;
}
