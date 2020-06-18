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

#include "QCtmLogManager.h"
#include "QCtmLogEvent.h"
#include "QCtmAbstractLogModel.h"
#include "QCtmLogData.h"

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

Q_CONSTEXPR int OneDay = 60 * 60 * 24;
Q_CONSTEXPR const char* objExp = "#\\w+\\b";

void qtMessageHandle(QtMsgType type, const QMessageLogContext& context, const QString& msg);

struct QCtmLogManager::Impl
{
    QString logPath;
    LogSavePolicy policy{ Size };
    QVector<QCtmAbstractLogModel*> models;
    bool saveLogs[QtMsgType::QtInfoMsg + 1];
    QDateTime datetime;
    qint64 logSize{ 4 * 1024 * 1024 };
    QFile logFile;
    QMutex mutex;
    
    static decltype(&qtMessageHandle) oldHandle;
};

decltype(&qtMessageHandle) QCtmLogManager::Impl::oldHandle;

/*!
    \class      QCtmLogManager
    \brief      QCtmLogManager hook the qt debug frame and manager the debug messages.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \enum       QCtmLogManager::LogSavePolicy
                Describe save policy of log.
    \value      Date
                Segmentation the log file by date.
    \value      Size
                Segmentation the log file by size.
*/

/*!
    \brief      Call the function before the QApplication is instantiated. If this function is not be called, the
                debug message will not be handle.
*/
void QCtmLogManager::initBeforeApp()
{
    Impl::oldHandle = qInstallMessageHandler(&qtMessageHandle);
}

/*!
    \brief      Returns the log manager singleton instance.
*/
QCtmLogManager& QCtmLogManager::instance()
{
    static QCtmLogManager ins;
    return ins;
}

/*!
    \brief      Set the log file \a path.
    \sa         logFilePath
*/
void QCtmLogManager::setLogFilePath(const QString& path)
{
    m_impl->logPath = path;
}

/*!
    \brief      Returns the log file path.
    \sa         setLogFilePath
*/
const QString& QCtmLogManager::logFilePath() const
{
    return m_impl->logPath;
}

/*!
    \brief      Sets save \a policy of log.
    \sa         logSavePolicy
*/
void QCtmLogManager::setLogSavePolicy(LogSavePolicy policy)
{
    m_impl->policy = policy;
}

/*!
    \brief      Returns the save policy of log.
    \sa         setLogSavePolicy
*/
QCtmLogManager::LogSavePolicy QCtmLogManager::logSavePolicy() const
{
    return m_impl->policy;
}

/*!
    \brief      Sets whether the log message \a type should be \a save.
    \sa         logTypeEnable
*/
void QCtmLogManager::setLogTypeEnable(QtMsgType type, bool save)
{
    m_impl->saveLogs[type] = save;
}

/*!
    \brief      Returns whether the log message \a type should be save.
    \sa         setLogTypeEnable
*/
bool QCtmLogManager::logTypeEnable(QtMsgType type) const
{
    return m_impl->saveLogs[type];
}

/*!
    \brief      Set the log file limit \a size, if the save policy is LogSaveType::Size.
    \sa         logSizeLimit
*/
void QCtmLogManager::setLogSizeLimit(qint64 size)
{
    m_impl->logSize = size;
}

/*!
    \brief      Returns limit size of the log file.
    \sa         setLogSizeLimit
*/
qint64 QCtmLogManager::logSizeLimit() const
{
    return m_impl->logSize;
}

void qtMessageHandle(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString message = msg;
    const auto objList = QCtmLogManager::parseObjectNames(message);
    auto data = std::make_shared<QCtmLogData>(type, context, message);

    for (auto model : QCtmLogManager::instance().m_impl->models)
    {
        if (objList.contains(model->objectName()))
        {
            if (QThread::currentThread() == qApp->thread())
            {
                auto evt = new QCtmLogEvent(data);
                qApp->sendEvent(model, evt);
            }
            else
            {
                auto evt = new QCtmLogEvent(data);
                qApp->postEvent(model, evt);
            }
        }
    }

    QCtmLogManager::Impl::oldHandle(type, context, message);

    QMutexLocker locker(&QCtmLogManager::instance().m_impl->mutex);
    if (QCtmLogManager::instance().m_impl->saveLogs[type])
    {
        QCtmLogManager::instance().writeLog(data);
    }
}

/*!
    \brief      Constructs a log manager.
*/
QCtmLogManager::QCtmLogManager()
    : m_impl(std::make_unique<Impl>())
{
    m_impl->datetime = QDateTime::currentDateTime();
    m_impl->logPath = qApp->applicationDirPath() + "/logs";
	QDir dir(m_impl->logPath);
	if (!dir.exists())
	{
		dir.mkpath(m_impl->logPath);
	}
    for (int i = 0; i < sizeof(m_impl->saveLogs) / sizeof(bool); i++)
    {
        m_impl->saveLogs[i] = true;
    }
}

/*!
    \brief      Destorys the log manager.
*/
QCtmLogManager::~QCtmLogManager()
{
}

/*!
    \brief      Write the log message \a data to file.
*/
void QCtmLogManager::writeLog(QCtmLogDataPtr data)
{
    QString level;
    switch (data->type())
    {
    case QtMsgType::QtInfoMsg:
        level = "Info";
        break;
    case QtMsgType::QtWarningMsg:
        level = "Warn";
        break;
    case QtMsgType::QtCriticalMsg:
        level = "Error";
        break;
    case QtMsgType::QtFatalMsg:
        level = "Abort";
        break;
    case QtMsgType::QtDebugMsg:
        level = "Debug";
        break;
    }
    if (checkFile())
    {
        auto log = QString("[%1] [%4] [%2:%3] %5\n")
            .arg(data->dateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))
            .arg(data->context().file)
            .arg(data->context().line)
            .arg(level)
            .arg(data->msg());
        m_impl->logFile.write(log.toLocal8Bit());
        m_impl->logFile.flush();
    }
    else
    {
        auto err = m_impl->logFile.errorString();
    }
}

/*!
    \brief      Register the log message \a model to show log message on views.
    \sa         unRegisterModel
*/
void QCtmLogManager::registerModel(QCtmAbstractLogModel* model)
{
    m_impl->models.push_back(model);
}

/*!
    \brief      Unregister the log message \a model.
    \sa         registerModel
*/
void QCtmLogManager::unRegisterModel(QCtmAbstractLogModel* model)
{
    m_impl->models.removeOne(model);
}

/*!
    \brief      Parse object names in the \a msg.
*/
QList<QString> QCtmLogManager::parseObjectNames(QString& msg)
{
    QRegExp rx(objExp);
    QStringList list;
    int pos = 0;

    QString temp = msg;
    while ((pos = rx.indexIn(msg, pos)) != -1) {
        const auto &str = rx.cap(0);
        list << str.right(str.size() - 1);
        pos += rx.matchedLength();
        temp = msg.right(msg.size() - pos);
    }

    if (!list.isEmpty())
    {
        if (!temp.isEmpty())
        {
            if (temp.at(0) == ' ')
            {
                temp = temp.right(temp.size() - 1);
            }
        }
    }

    msg = temp;
    return list;
}

/*!
    \brief      Check the log file, and create a new log file by save policy.
                Returns true if current log file is legal.
*/
bool QCtmLogManager::checkFile()
{
    switch (QCtmLogManager::instance().m_impl->policy)
    {
    case Date:
        if (m_impl->datetime.daysTo(QDateTime::currentDateTime()) >= 1 || !m_impl->logFile.isOpen())
        {
            m_impl->datetime = QDateTime::currentDateTime();
            if (m_impl->logFile.isOpen())
                m_impl->logFile.close();
            m_impl->logFile.setFileName(m_impl->logPath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh.mm.ss") + ".log");
            return m_impl->logFile.open(QFile::WriteOnly | QFile::Append);
        }
        break;
    case Size:
        if (!m_impl->logFile.isOpen() || m_impl->logFile.size() >= m_impl->logSize)
        {
            if (m_impl->logFile.isOpen())
                m_impl->logFile.close();
            const auto& file = m_impl->logPath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh.mm.ss.zzz") + ".log";
            m_impl->logFile.setFileName(file);
            return m_impl->logFile.open(QFile::WriteOnly | QFile::Append);
        }
        break;
    }
    return true;
}
