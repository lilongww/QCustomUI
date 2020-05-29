#pragma once

#include "qcustomui_global.h"

#include <QString>

#include <memory>

class QCtmAbstractLogModel;
struct QCtmLogData;
typedef std::shared_ptr<QCtmLogData> QCtmLogDataPtr;

class QCUSTOMUI_EXPORT QCtmLogManager
{
public:
    enum LogSaveType
    {
        Date,
        Size
    };

    static void initBeforeApp();
    static QCtmLogManager& instance();
    void setLogFilePath(const QString& path);
    const QString& logFilePath() const;
    void setLogSaveType(LogSaveType type);
    LogSaveType logSaveType() const;
    void setLogTypeEnable(QtMsgType type, bool save);
    bool logTypeEnable(QtMsgType type) const;
    void setLogSizeLimit(qint64 size);
    qint64 logSizeLimit() const;
protected:
    QCtmLogManager();
    ~QCtmLogManager();

    virtual bool checkFile();
    virtual void writeLog(QCtmLogDataPtr data);

    void registerModel(QCtmAbstractLogModel* model);
    void unRegisterModel(QCtmAbstractLogModel* model);

    static QList<QString> parseObjectNames(QString& msg);
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    friend QCtmAbstractLogModel;
    friend void qtMessageHandle(QtMsgType, const QMessageLogContext&, const QString&);
};

