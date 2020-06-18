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

#include "QCtmMessageModel.h"
#include "QCtmMessage.h"

#include <QColor>

struct QCtmMessageModel::Impl
{
	QColor titleColor;
	QColor timeColor;
};

QCtmMessageModel::QCtmMessageModel(QObject *parent)
	: QCtmAbstractMessageTipModel(parent)
	, m_impl(std::make_unique<Impl>())
{
}

QCtmMessageModel::~QCtmMessageModel()
{
}

QVariant QCtmMessageModel::data(const QModelIndex& index, int role) const
{
	QVariant d;
	if (index.row() == -1)
		return d;
	auto msg = std::dynamic_pointer_cast<QCtmMessage>(message(index.row()));
	if (msg)
	{
		if (role == Qt::DisplayRole)
		{
			switch (index.column())
			{
			case QCtmMessage::Title:
				d = msg->title();
				break;
			case QCtmMessage::Content:
				d = msg->content();
				break;
			case QCtmMessage::Time:
				d = msg->dateTime().toString("yyyy-MM-dd hh:mm:ss");
				break;
			}
		}
		else if (role == Qt::ForegroundRole)
		{
			switch (index.column())
			{
			case QCtmMessage::Title:
				d = QColor(m_impl->titleColor);
				break;
			case QCtmMessage::Content:
				break;
			case QCtmMessage::Time:
				d = QColor(m_impl->timeColor);
				break;
			}
		}
		else if (role == Qt::ToolTipRole)
		{
			d = QString("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p><p>%2</p><p>%3</p></body></html>")
				.arg(msg->title())
				.arg(msg->content())
				.arg(msg->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
		}
	}
	return d;
}

int QCtmMessageModel::columnCount([[maybe_unused]] const QModelIndex &parent /* = QModelIndex() */) const
{
	return QCtmMessage::ColumnCount;
}

/**
 * @brief   Set the color of the message title.
 */
void QCtmMessageModel::setTitleColor(const QColor& color)
{
	m_impl->titleColor = color;
}

/**
 * @brief	Get the color of the message title.
 */
const QColor& QCtmMessageModel::titleColor() const
{
	return m_impl->titleColor;
}

/**
 * @brief	Set the color of the message time.
 */
void QCtmMessageModel::setTimeColor(const QColor& color)
{
	m_impl->timeColor = color;
}

/**
 * @brief	Get the color of the message time.
 */
const QColor& QCtmMessageModel::timeColor() const
{
	return m_impl->timeColor;
}
