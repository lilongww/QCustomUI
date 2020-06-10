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

#pragma once

#include "qcustomui_global.h"

#include <QString>
#include <QDateTime>

#include <memory>

class QCUSTOMUI_EXPORT QCtmAbstractMessage
{
public:
	QCtmAbstractMessage();
	virtual ~QCtmAbstractMessage() {}

};

class QCUSTOMUI_EXPORT QCtmMessage :public QCtmAbstractMessage
{
public:
	enum Column
	{
		Title,
		Content,
		Time,
		ColumnCount
	};

	QCtmMessage(const QString& title, const QString& content, const QDateTime& time);
	~QCtmMessage();

	void setTitle(const QString& title);
	const QString& title()const;
	void setContent(const QString& content);
	const QString& content()const;
	void setDateTime(const QDateTime& time);
	const QDateTime& dateTime()const;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

