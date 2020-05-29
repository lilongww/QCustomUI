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

