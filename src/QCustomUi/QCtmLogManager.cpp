/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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
#include "QCtmAbstractLogModel.h"
#include "QCtmLogData.h"
#include "QCtmLogEvent.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QRegularExpression>
#include <QThread>

Q_CONSTEXPR int OneDay         = 60 * 60 * 24;
Q_CONSTEXPR const char* objExp = "#\\w+\\b";

void qtMessageHandle(QtMsgType type, const QMessageLogContext& context, const QString& msg);

struct QCtmLogManager::Impl
{
    QString logPath;
    LogSavePolicy policy { Size };
    QVector<QCtmAbstractLogModel*> models;
    bool saveLogs[QtMsgType::QtInfoMsg + 1];
    QDateTime datetime;
    qint64 logSize { 4 * 1024 * 1024 };
    QFile logFile;
    QMutex mutex;

    static decltype(&qtMessageHandle) oldHandle;
};

decltype(&qtMessageHandle) QCtmLogManager::Impl::oldHandle;

/*!
    \class      QCtmLogManager
    \brief      日志管理器.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLogManager.h
*/

/*!
    \enum       QCtmLogManager::LogSavePolicy
                日志保存策略.
    \value      Date
                按日期保存.
    \value      Size
                按大小保存.
*/

/*!
    \brief      在 qApp 初始化之前调用该函数重定向日志句柄，如果在初始化之后调用则无法获取日志.
*/
void QCtmLogManager::initBeforeApp() { Impl::oldHandle = qInstallMessageHandler(&qtMessageHandle); }

/*!
    \brief      返回单例.
*/
QCtmLogManager& QCtmLogManager::instance()
{
    static QCtmLogManager ins;
    return ins;
}

/*!
    \brief      设置日志保存路径 \a path.
    \sa         logFilePath
*/
void QCtmLogManager::setLogFilePath(const QString& path) { m_impl->logPath = path; }

/*!
    \brief      返回日志保存路径.
    \sa         setLogFilePath
*/
const QString& QCtmLogManager::logFilePath() const { return m_impl->logPath; }

/*!
    \brief      设置日志保存策略 \a policy.
    \sa         logSavePolicy
*/
void QCtmLogManager::setLogSavePolicy(LogSavePolicy policy) { m_impl->policy = policy; }

/*!
    \brief      返回日志保存策略.
    \sa         setLogSavePolicy
*/
QCtmLogManager::LogSavePolicy QCtmLogManager::logSavePolicy() const { return m_impl->policy; }

/*!
    \brief      设置日志类型 \a type 是否保存 \a save.
    \sa         logTypeEnable
*/
void QCtmLogManager::setLogTypeEnable(QtMsgType type, bool save) { m_impl->saveLogs[type] = save; }

/*!
    \brief      返回日志类型 \a type 是否保存.
    \sa         setLogTypeEnable
*/
bool QCtmLogManager::logTypeEnable(QtMsgType type) const { return m_impl->saveLogs[type]; }

/*!
    \brief      设置日志大小限制 \a size.
    \sa         logSizeLimit
*/
void QCtmLogManager::setLogSizeLimit(qint64 size) { m_impl->logSize = size; }

/*!
    \brief      返回日志大小限制.
    \sa         setLogSizeLimit
*/
qint64 QCtmLogManager::logSizeLimit() const { return m_impl->logSize; }

void qtMessageHandle(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString message    = msg;
    const auto objList = QCtmLogManager::parseObjectNames(message);
    auto data          = std::make_shared<QCtmLogData>(type, context, message);

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMutexLocker locker(&QCtmLogManager::instance().m_impl->mutex);
#else
    QMutexLocker<QMutex> locker(&QCtmLogManager::instance().m_impl->mutex);
#endif
    if (QCtmLogManager::instance().m_impl->saveLogs[type])
    {
        QCtmLogManager::instance().writeLog(data);
    }
}

/*!
    \brief      构造函数.
*/
QCtmLogManager::QCtmLogManager() : m_impl(std::make_unique<Impl>())
{
    m_impl->datetime = QDateTime::currentDateTime();
    m_impl->logPath  = qApp->applicationDirPath() + "/logs";
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
    \brief      析构函数.
*/
QCtmLogManager::~QCtmLogManager() {}

/*!
    \brief      写入日志 \a data.
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
    \brief      设置日志 \a model.
    \sa         unRegisterModel
*/
void QCtmLogManager::registerModel(QCtmAbstractLogModel* model) { m_impl->models.push_back(model); }

/*!
    \brief      移除日志 \a model.
    \sa         registerModel
*/
void QCtmLogManager::unRegisterModel(QCtmAbstractLogModel* model) { m_impl->models.removeOne(model); }

/*!
    \brief      解析日志 \a msg.
*/
QList<QString> QCtmLogManager::parseObjectNames(QString& msg)
{
    QRegularExpression rx(objExp);
    QRegularExpressionMatchIterator i = rx.globalMatch(msg);
    QStringList list;

    while (i.hasNext())
    {
        auto match      = i.next();
        const auto& str = match.captured();
        list << str.right(str.size() - 1);
    }

    if (!list.isEmpty())
    {
        for (const auto& objName : list)
        {
            msg.replace("#" + objName, "");
        }
    }

    return list;
}

/*!
    \brief      检测日志文件，根据日志保存策略重新保存日志.
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
