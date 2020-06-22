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

#include "QCtmLogModel.h"
#include "QCtmLogEvent.h"
#include "QCtmLogData.h"

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

    QCtmLogData::LogInsertPolicy logInsertPolicy{ QCtmLogData::LogInsertPolicy::ASC };
};

/*!
    \class      QCtmLogModel
    \brief      QCtmLogModel provide a log message model to show log message datas in view.
    \inherits   QCtmAbstractLogModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a model with \a objectName and \a parent.
    \sa         QObject::setObjectName
*/
QCtmLogModel::QCtmLogModel(const QString& objectName, QObject* parent)
    : QCtmAbstractLogModel(objectName, parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->headers << tr("Lv") << tr("DateTime") << tr("Description");
}

/*!
    \brief      Destroys the model.
*/
QCtmLogModel::~QCtmLogModel()
{
}

/*!
    \brief      Clear all log messages.
*/
void QCtmLogModel::clear()
{
    beginResetModel();
    m_impl->datas.clear();
    m_impl->infoCount = 0;
    m_impl->errorCount = 0;
    m_impl->warningCount = 0;
    endResetModel();
}

/*!
    \reimp
*/
QVariant QCtmLogModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    auto& msg = m_impl->datas[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch ((Column)index.column())
        {
        case Column::DateTime:
            return msg.dateTime.toString("yyyy-MM-dd hh:mm:ss");
        case Column::Message:
            return msg.msg;
        default:
            break;
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
            default:
                break;
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
    return QVariant();
}

/*!
    \reimp
*/
bool QCtmLogModel::setData([[maybe_unused]] const QModelIndex& index, [[maybe_unused]] const QVariant& value, [[maybe_unused]] int role /* = Qt::EditRole */)
{
    return false;
}

/*!
    \reimp
*/
QVariant QCtmLogModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return m_impl->headers[section];
    }
    return QVariant();
}

/*!
    \reimp
*/
int QCtmLogModel::rowCount([[maybe_unused]] const QModelIndex& parent /*= QModelIndex()*/) const
{
    return m_impl->datas.size();
}

/*!
    \reimp
*/
int QCtmLogModel::columnCount([[maybe_unused]] const QModelIndex& parent /*= QModelIndex()*/) const
{
    return m_impl->headers.size();
}

/*!
    \brief      Sets maximum message \a count of the log.
    \sa         maximumCount()
*/
void QCtmLogModel::setMaximumCount(int count)
{
    m_impl->maxCount = count;
}

/*!
    \brief      Returns maximum message count of the log.
    \sa         setMaximumCount
*/
int QCtmLogModel::maximumCount() const
{
    return m_impl->maxCount;
}

/*!
    \brief      Sets insert \a policy of the log.
    \sa         logInsertPolicy()
*/
void QCtmLogModel::setLogInsertPolicy(QCtmLogData::LogInsertPolicy policy)
{
    m_impl->logInsertPolicy = policy;
}

/*!
    \brief      Returns insert policy of the log.
    \sa         setLogInsertPolicy
*/
QCtmLogData::LogInsertPolicy QCtmLogModel::logInsertPolicy() const
{
    return m_impl->logInsertPolicy;
}

/*!
    \brief      Sets the \a icon for information log message type.
    \sa         infoIcon()
*/
void QCtmLogModel::setInfoIcon(const QIcon& icon)
{
    m_impl->infoIcon = icon;
}

/*!
    \brief      Returns the icon for information log message.
    \sa         setInfoIcon
*/
const QIcon& QCtmLogModel::infoIcon() const
{
    return m_impl->infoIcon;
}

/*!
    \brief      Sets the \a icon for warning log message type.
    \sa         warningIcon()
*/
void QCtmLogModel::setWarningIcon(const QIcon& icon)
{
    m_impl->warningIcon = icon;
}

/*!
    \brief      Returns the icon for warning log message type.
    \sa         setWarningIcon
*/
const QIcon& QCtmLogModel::warningIcon() const
{
    return m_impl->warningIcon;
}

/*!
    \brief      Sets the \a icon for error log message type.
    \sa         errorIcon()
*/
void QCtmLogModel::setErrorIcon(const QIcon& icon)
{
    m_impl->errorIcon = icon;
}

/*!
    \brief      Returns the icon for error log message type.
    \sa         setErrorIcon
*/
const QIcon& QCtmLogModel::errorIcon() const
{
    return m_impl->errorIcon;
}

/*!
    \brief      Returns the count for warning log message type.
*/
int QCtmLogModel::warningCount() const
{
    return m_impl->warningCount;
}

/*!
    \brief      Returns the count for information log message type.
*/
int QCtmLogModel::infoCount() const
{
    return m_impl->infoCount;
}

/*!
    \brief      Returns the count for error log message type.
*/
int QCtmLogModel::errorCount() const
{
    return m_impl->errorCount;
}

/*!
    \reimp
*/
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
        default:
            continue;
        }
        beginRemoveRows(QModelIndex(), 0, 0);
        if (m_impl->logInsertPolicy == QCtmLogData::LogInsertPolicy::ASC)
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
    default:
        break;
    }
    beginInsertRows(QModelIndex(), m_impl->datas.size(), m_impl->datas.size());
    if (m_impl->logInsertPolicy == QCtmLogData::LogInsertPolicy::ASC)
        m_impl->datas.push_back(msg);
    else
        m_impl->datas.push_front(msg);
    endInsertRows();
}

/*!
    \brief      Update the header texts when the language has been changed.
*/
void QCtmLogModel::retranslateUi()
{
    m_impl->headers.clear();
    m_impl->headers << tr("Lv") << tr("DateTime") << tr("Description");
}
