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

#include "QCtmLogData.h"

/*!
    \class      QCtmLogData
    \brief      Log message data definition.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/


/*!
    \enum       QCtmLogData::LogInsertPolicy
                Log inert policy
    \value      ASC
                Ascending order.
    \value      DESC
                Descending order.
*/

/*!
    \brief      Constructs a log message data with \a type, \a context, \a msg.
*/
QCtmLogData::QCtmLogData(QtMsgType type, const QMessageLogContext& context, const QString& msg)
    : m_type(type)
    , m_msg(msg)
    , m_dateTime(QDateTime::currentDateTime())
{
    m_context.category = context.category;
    m_context.file = context.file;
    m_context.line = context.line;
    m_context.function = context.function;
}

/*!
    \brief      Destroys the log message data.
*/
QCtmLogData::~QCtmLogData()
{

}

/*!
    \brief      Returns type of the log message data.
*/
QtMsgType QCtmLogData::type() const
{
    return m_type;
}

/*!
    \brief      Returns context of the log message data.
*/
const QMessageLogContext& QCtmLogData::context() const
{
    return m_context;
}

/*!
    \brief      Returns message content of the log message data.
*/
const QString& QCtmLogData::msg() const
{
    return m_msg;
}

/*!
    \brief      Returns date time of the log message data.
*/
const QDateTime& QCtmLogData::dateTime() const
{
    return m_dateTime;
}
