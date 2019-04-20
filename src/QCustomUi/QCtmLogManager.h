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

    /**
     * @brief		
     * @param[in]	
     * @param[in]	
     * @Return:		
     */
    static void initBeforeApp();

    /**
     * @brief		获取日志管理器实例，不要在qApp实例化之前调用该函数
     */
    static QCtmLogManager& instance();

    /**
     * @brief		设置日志路径
     * @param[in]	path 日志路径
     */
    void setLogFilePath(const QString& path);

    /**
     * @brief		获取日志路径
     */
    const QString& logFilePath() const;

    /**
     * @brief		设置日志保存类型
     * @param[in]	日志保存类型
     */
    void setLogSaveType(LogSaveType type);

    /**
     * @brief		获取日志保存类型
     */
    LogSaveType logSaveType() const;
    
    /**
     * @brief		设置哪些日志需要保存
     * @param[in]	type 日志登记
     * @param[in]	save true:保存，false:不保存
     */
    void setLogTypeEnable(QtMsgType type, bool save);

    /**
     * @brief		获取某日志类型是否保存
     * @param[in]	type 日志类型
     */
    bool logTypeEnable(QtMsgType type) const;

    /**
     * @brief		设置文件大小限制，只有在LogSaveType::Size模式下有效
     * @param[in]	size 日志文件超过该大小，新建一个日志文件
     */
    void setLogSizeLimit(qint64 size);

    /**
     * @brief		获取日志文件大小限制
     */
    qint64 logSizeLimit() const;
protected:
    QCtmLogManager();
    ~QCtmLogManager();

    /**
     * @brief		检查log文件，并按照一定规则建立log文件
     */
    virtual bool checkFile();
    
    /**
     * @brief		将日志写入日志文件
     * @param[in]	data 日志信息
     */
    virtual void writeLog(QCtmLogDataPtr data);

    void registerModel(QCtmAbstractLogModel* model);
    void unRegisterModel(QCtmAbstractLogModel* model);

    static QList<QString> parseObjectNames(QString& msg);
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;

    friend QCtmAbstractLogModel;
    friend void qtMessageHandle(QtMsgType, const QMessageLogContext&, const QString&);
};

