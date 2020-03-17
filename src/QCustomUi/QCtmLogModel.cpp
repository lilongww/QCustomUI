#include "QCtmLogModel.h"
#include "QCtmLogEvent.h"

#include <QDebug>

enum class Column
{
    Level,
    DateTime,
    Message
};

struct QCtmLogModel::Impl
{
    QList<QCtmLogMessage> datas;
    QList<QString> headers;
    QIcon infoIcon;
    QIcon warningIcon;
    QIcon errorIcon;
	int maxCount{ 10000 };

    int errorCount{ 0 };
    int warningCount{ 0 };
    int infoCount{ 0 };

    LogInsertMode logInsertMode{ ASC };
};

QCtmLogModel::QCtmLogModel(const QString& objectName, QObject *parent)
    : QCtmAbstractLogModel(objectName, parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->headers << tr("Lv") << tr("DateTime") << tr("Description");
}

QCtmLogModel::~QCtmLogModel()
{
}

void QCtmLogModel::clear()
{
    beginResetModel();
    m_impl->datas.clear();
    m_impl->infoCount = 0;
    m_impl->errorCount = 0;
    m_impl->warningCount = 0;
    endResetModel();
}

QVariant QCtmLogModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    auto &msg = m_impl->datas[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch ((Column)index.column())
        {
        case Column::DateTime:
            return msg.dateTime.toString("yyyy-MM-dd hh:mm:ss");
        case Column::Message:
            return msg.msg;
        }
    }
    else if (role == TypeRole)
    {
        return msg.type;
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            switch (msg.type)
            {
            case QtMsgType::QtInfoMsg:
                return m_impl->infoIcon;
            case QtMsgType::QtWarningMsg:
                return m_impl->warningIcon;
            case QtMsgType::QtCriticalMsg:
                return m_impl->errorIcon;
            }
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        if (index.column() == 2)
        {
            return msg.msg;
        }
    }
 //   else if (role == Qt::ForegroundRole)
	//{
	//	switch (msg.type)
	//	{
	//	case QtMsgType::QtInfoMsg:
 //           return QColor(Qt::green);
	//	case QtMsgType::QtWarningMsg:
	//		return QColor(Qt::yellow);
	//	case QtMsgType::QtCriticalMsg:
 //           return QColor(Qt::red);
	//	}
 //   }
    return QVariant();
}

bool QCtmLogModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    return false;
}

QVariant QCtmLogModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
    if (role == Qt::DisplayRole&&orientation == Qt::Horizontal)
    {
        return m_impl->headers[section];
    }
    return QVariant();
}

int QCtmLogModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return m_impl->datas.size();
}

int QCtmLogModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return m_impl->headers.size();
}

void QCtmLogModel::setMaximumCount(int count)
{
	m_impl->maxCount = count;
}

int QCtmLogModel::maximumCount() const
{
	return m_impl->maxCount;
}

void QCtmLogModel::setLogInsertMode(LogInsertMode mode)
{
    m_impl->logInsertMode = mode;
}

LogInsertMode QCtmLogModel::logInsertMode() const
{
    return m_impl->logInsertMode;
}

void QCtmLogModel::setInfoIcon(const QIcon& icon)
{
    m_impl->infoIcon = icon;
}

const QIcon& QCtmLogModel::infoIcon() const
{
    return m_impl->infoIcon;
}

void QCtmLogModel::setWarningIcon(const QIcon& icon)
{
    m_impl->warningIcon = icon;
}

const QIcon& QCtmLogModel::warningIcon() const
{
    return m_impl->warningIcon;
}

void QCtmLogModel::setErrorIcon(const QIcon& icon)
{
    m_impl->errorIcon = icon;
}

const QIcon& QCtmLogModel::errorIcon() const
{
    return m_impl->errorIcon;
}

int QCtmLogModel::warningCount() const
{
    return m_impl->warningCount;
}

int QCtmLogModel::infoCount() const
{
    return m_impl->infoCount;
}

int QCtmLogModel::errorCount() const
{
    return m_impl->errorCount;
}

void QCtmLogModel::logEvent(QCtmLogEvent* e)
{
	while (m_impl->datas.size() >= m_impl->maxCount && !m_impl->datas.isEmpty())
	{
        auto msg = m_impl->datas.front();
        switch (msg.type)
        {
        case QtMsgType::QtInfoMsg:
            m_impl->infoCount--;
            break;
        case QtMsgType::QtWarningMsg:
            m_impl->warningCount--;
            break;
        case QtMsgType::QtCriticalMsg:
            m_impl->errorCount--;
            break;
        }
        beginRemoveRows(QModelIndex(), 0, 0);
        if (m_impl->logInsertMode == ASC)
            m_impl->datas.removeFirst();
        else
            m_impl->datas.removeLast();
        endRemoveRows();
	}

	if (m_impl->maxCount <= 0)
		return;

    QCtmLogMessage msg;
    msg.dateTime = e->log()->dateTime();
    msg.msg = e->log()->msg();
    msg.type = e->log()->type();
    switch (msg.type)
    {
    case QtMsgType::QtInfoMsg:
        m_impl->infoCount++;
        break;
    case QtMsgType::QtWarningMsg:
        m_impl->warningCount++;
        break;
    case QtMsgType::QtCriticalMsg:
        m_impl->errorCount++;
        break;
    }
    beginInsertRows(QModelIndex(), m_impl->datas.size(), m_impl->datas.size());
    if (m_impl->logInsertMode == ASC)
        m_impl->datas.push_back(msg);
    else
        m_impl->datas.push_front(msg);
    endInsertRows();
}

void QCtmLogModel::retranslateUi()
{
    m_impl->headers.clear();
    m_impl->headers << tr("Lv") << tr("DateTime") << tr("Description");
}
