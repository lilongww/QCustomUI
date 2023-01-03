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

#include <QDateTime>
#include <qlogging.h>

#include <memory>

class QCUSTOMUI_EXPORT QCtmLogData
{
public:
    enum LogInsertPolicy
    {
        ASC,
        DESC
    };
    QCtmLogData(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    ~QCtmLogData();
    QtMsgType type() const;
    const QMessageLogContext& context() const;
    const QString& msg() const;
    const QDateTime& dateTime() const;

private:
    QtMsgType m_type;
    QMessageLogContext m_context;
    QString m_msg;
    QDateTime m_dateTime;
};

using QCtmLogDataPtr = std::shared_ptr<QCtmLogData>;
