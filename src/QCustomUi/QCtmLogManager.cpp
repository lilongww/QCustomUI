#include "QCtmLogManager.h"
#include "QCtmLogEvent.h"
#include "QCtmAbstractLogModel.h"

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
    LogSaveType type{ Size };
    QVector<QCtmAbstractLogModel*> models;
    bool saveLogs[QtMsgType::QtInfoMsg + 1];
    QDateTime datetime;
    qint64 logSize{ 4 * 1024 * 1024 };
    QFile logFile;
    QMutex mutex;
    
    static decltype(&qtMessageHandle) oldHandle;
};

decltype(&qtMessageHandle) QCtmLogManager::Impl::oldHandle;

/**
 * @brief
 * @param[in]
 * @param[in]
 * @Return:
 */
void QCtmLogManager::initBeforeApp()
{
    Impl::oldHandle = qInstallMessageHandler(&qtMessageHandle);
}

/**
 * @brief		获取日志管理器实例，不要在qApp实例化之前调用该函数
 */
QCtmLogManager& QCtmLogManager::instance()
{
    static QCtmLogManager ins;
    return ins;
}

/**
 * @brief		设置日志路径
 * @param[in]	path 日志路径
 */
void QCtmLogManager::setLogFilePath(const QString& path)
{
    m_impl->logPath = path;
}

/**
 * @brief		获取日志路径
 */
const QString& QCtmLogManager::logFilePath() const
{
    return m_impl->logPath;
}

/**
 * @brief		设置日志保存类型
 * @param[in]	日志保存类型
 */
void QCtmLogManager::setLogSaveType(LogSaveType type)
{
    m_impl->type = type;
}

/**
 * @brief		获取日志保存类型
 */
QCtmLogManager::LogSaveType QCtmLogManager::logSaveType() const
{
    return m_impl->type;
}

/**
 * @brief		设置哪些日志需要保存
 * @param[in]	type 日志登记
 * @param[in]	save true:保存，false:不保存
 */
void QCtmLogManager::setLogTypeEnable(QtMsgType type, bool save)
{
    m_impl->saveLogs[type] = save;
}

/**
 * @brief		获取某日志类型是否保存
 * @param[in]	type 日志类型
 */
bool QCtmLogManager::logTypeEnable(QtMsgType type) const
{
    return m_impl->saveLogs[type];
}

/**
 * @brief		设置文件大小限制，只有在LogSaveType::Size模式下有效
 * @param[in]	size 日志文件超过该大小，新建一个日志文件
 */
void QCtmLogManager::setLogSizeLimit(qint64 size)
{
    m_impl->logSize = size;
}

/**
 * @brief		获取日志文件大小限制
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


QCtmLogManager::~QCtmLogManager()
{
}

/**
 * @brief		将日志写入日志文件
 * @param[in]	data 日志信息
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

void QCtmLogManager::registerModel(QCtmAbstractLogModel* model)
{
    m_impl->models.push_back(model);
}

void QCtmLogManager::unRegisterModel(QCtmAbstractLogModel* model)
{
    m_impl->models.removeOne(model);
}

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

/**
 * @brief		检查log文件，并按照一定规则建立log文件
 */
bool QCtmLogManager::checkFile()
{
    switch (QCtmLogManager::instance().m_impl->type)
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
