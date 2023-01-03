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

#include "QCtmLogData.h"

/*!
    \class      QCtmLogData
    \brief      日志消息结构定义.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmLogData.h
*/

/*!
    \enum       QCtmLogData::LogInsertPolicy
                日志插入策略.
    \value      ASC
                正序插入.
    \value      DESC
                逆序插入.
*/

/*!
    \brief      构造函数 \a type, \a context, \a msg.
*/
QCtmLogData::QCtmLogData(QtMsgType type, const QMessageLogContext& context, const QString& msg)
    : m_type(type), m_msg(msg), m_dateTime(QDateTime::currentDateTime())
{
    m_context.category = context.category;
    m_context.file     = context.file;
    m_context.line     = context.line;
    m_context.function = context.function;
}

/*!
    \brief      析构函数.
*/
QCtmLogData::~QCtmLogData() {}

/*!
    \brief      返回日志类型.
*/
QtMsgType QCtmLogData::type() const { return m_type; }

/*!
    \brief      返回日志上下文.
*/
const QMessageLogContext& QCtmLogData::context() const { return m_context; }

/*!
    \brief      返回日志内容.
*/
const QString& QCtmLogData::msg() const { return m_msg; }

/*!
    \brief      返回日志时间.
*/
const QDateTime& QCtmLogData::dateTime() const { return m_dateTime; }
