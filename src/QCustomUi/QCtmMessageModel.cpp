#include "QCtmMessageModel.h"
#include "QCtmMessage.h"

#include <QColor>

struct QCtmMessageModel::Impl
{
	QColor titleColor;
	QColor timeColor;
};

QCtmMessageModel::QCtmMessageModel(QObject *parent)
	: QCtmAbstractMessageModel(parent)
	, m_impl(std::make_shared<Impl>())
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
		else if (role == Qt::TextColorRole)
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

int QCtmMessageModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	return QCtmMessage::ColumnCount;
}

void QCtmMessageModel::setTitleColor(const QColor& color)
{
	m_impl->titleColor = color;
}

const QColor& QCtmMessageModel::titleColor() const
{
	return m_impl->titleColor;
}

void QCtmMessageModel::setTimeColor(const QColor& color)
{
	m_impl->timeColor = color;
}

const QColor& QCtmMessageModel::timeColor() const
{
	return m_impl->timeColor;
}
