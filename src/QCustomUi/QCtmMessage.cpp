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
	:m_impl(std::make_shared<Impl>())
{
	m_impl->title = title;
	m_impl->content = content;
	m_impl->time = time;
}

QCtmMessage::~QCtmMessage()
{
}

void QCtmMessage::setTitle(const QString& title)
{
	m_impl->title = title;
}

const QString& QCtmMessage::title() const
{
	return m_impl->title;
}

void QCtmMessage::setContent(const QString& content)
{
	m_impl->content = content;
}

const QString& QCtmMessage::content() const
{
	return m_impl->content;
}

void QCtmMessage::setDateTime(const QDateTime& time)
{
	m_impl->time = time;
}

const QDateTime& QCtmMessage::dateTime() const
{
	return m_impl->time;
}
