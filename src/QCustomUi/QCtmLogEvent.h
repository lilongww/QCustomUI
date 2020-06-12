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

#pragma once

#include "qcustomui_global.h"

#include <QEvent>
#include <QString>
#include <QDateTime>

#include <memory>

struct QCUSTOMUI_EXPORT QCtmLogData
{
    QCtmLogData(QtMsgType type, const QMessageLogContext& context, const QString& msg)
        : m_type(type)
        , m_msg(msg)
        , m_dateTime(QDateTime::currentDateTime())
    {
        m_context.category = context.category;
        m_context.file = context.file;
        m_context.line = context.line;
        m_context.function = context.function;
    }
    ~QCtmLogData(){}

    /**
     * @brief		Get the log type.
     */
    QtMsgType type() const { return m_type; }
    
    /**
     * @brief		Get the log context.
     */
    const QMessageLogContext& context() const { return m_context; }

    /**
     * @brief		Get the log message.
     */
    const QString& msg() const { return m_msg; }

    /**
     * @brief		Get the log date time.
     */
    const QDateTime& dateTime() const { return m_dateTime; }
private:
    QtMsgType m_type;
    QMessageLogContext m_context;
    QString m_msg;
    QDateTime m_dateTime;
};
using QCtmLogDataPtr = std::shared_ptr<QCtmLogData>;

class QCUSTOMUI_EXPORT QCtmLogEvent : public QEvent
{
public:
    enum { QCtmLog = QEvent::User + 0xFE };
    QCtmLogEvent(QCtmLogDataPtr log);
    ~QCtmLogEvent();

    QCtmLogDataPtr log()const;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * @brief		The log insert order.
 */
enum LogInsertMode
{
    ASC,            //Ascending order.
    DESC            //Descending order.
};