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

#include "QCtmMessageTipData.h"

QCtmAbstractMessageTipData::QCtmAbstractMessageTipData() {}

QCtmAbstractMessageTipData::~QCtmAbstractMessageTipData() {}

struct QCtmMessageTipData::Impl
{
    QString title;
    QString content;
    QDateTime time;
};

/*!
    \class      QCtmMessageTipData
    \brief      标准消息提示数据结构定义.
    \inherits   QCtmAbstractMessageTipData
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmMessageTipData.h
*/

/*!
    \brief      构造函数 \a title, \a content, \a time.
*/
QCtmMessageTipData::QCtmMessageTipData(const QString& title, const QString& content, const QDateTime& time)
    : m_impl(std::make_unique<Impl>())
{
    m_impl->title   = title;
    m_impl->content = content;
    m_impl->time    = time;
}

/*!
    \brief      析构函数.
*/
QCtmMessageTipData::~QCtmMessageTipData() {}

/*!
    \brief      设置标题 \a title.
    \sa         title()
*/
void QCtmMessageTipData::setTitle(const QString& title) { m_impl->title = title; }

/*!
    \brief      返回标题.
    \sa         setTitle
*/
const QString& QCtmMessageTipData::title() const { return m_impl->title; }

/*!
    \brief      设置消息内容 \a content.
    \sa         content
*/
void QCtmMessageTipData::setContent(const QString& content) { m_impl->content = content; }

/*!
    \brief      返回消息内容.
    \sa         setContent
*/
const QString& QCtmMessageTipData::content() const { return m_impl->content; }

/*!
    \brief      设置消息时间 \a time.
    \sa         dateTime()
*/
void QCtmMessageTipData::setDateTime(const QDateTime& time) { m_impl->time = time; }

/*!
    \brief      返回消息时间.
    \sa         setDateTime
*/
const QDateTime& QCtmMessageTipData::dateTime() const { return m_impl->time; }
