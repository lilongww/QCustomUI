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

#pragma once

#include "qcustomui_global.h"

#include <QString>

#include <memory>

class QCtmAbstractLogModel;
using QCtmLogDataPtr = std::shared_ptr<class QCtmLogData>;

class QCUSTOMUI_EXPORT QCtmLogManager
{
public:
    enum LogSavePolicy
    {
        Date,
        Size
    };

    static void initBeforeApp();
    static QCtmLogManager& instance();
    void setLogFilePath(const QString& path);
    const QString& logFilePath() const;
    void setLogSavePolicy(LogSavePolicy policy);
    LogSavePolicy logSavePolicy() const;
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
