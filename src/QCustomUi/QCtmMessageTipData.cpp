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

#include "QCtmMessageTipData.h"

QCtmAbstractMessageTipData::QCtmAbstractMessageTipData()
{

}


QCtmAbstractMessageTipData::~QCtmAbstractMessageTipData()
{

}

struct QCtmMessageTipData::Impl
{
    QString title;
    QString content;
    QDateTime time;
};

/*!
    \class      QCtmMessageTipData
    \brief      QCtmMessageTipData provide a common message tip function.
    \inherits   QCtmAbstractMessageTipData
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a message tip data with \a title, \a content and \a time.
*/
QCtmMessageTipData::QCtmMessageTipData(const QString& title, const QString& content, const QDateTime& time)
    :m_impl(std::make_unique<Impl>())
{
    m_impl->title = title;
    m_impl->content = content;
    m_impl->time = time;
}

/*!
    \brief      Destroys the message tip data.
*/
QCtmMessageTipData::~QCtmMessageTipData()
{
}

/*!
    \brief      Sets \a title of the message tip data.
    \sa         title()
*/
void QCtmMessageTipData::setTitle(const QString& title)
{
    m_impl->title = title;
}

/*!
    \brief      Returns title of the message tip data.
    \sa         setTitle
*/
const QString& QCtmMessageTipData::title() const
{
    return m_impl->title;
}

/*!
    \brief      Sets \a content of the message tip data.
    \sa         content
*/
void QCtmMessageTipData::setContent(const QString& content)
{
    m_impl->content = content;
}

/*!
    \brief      Returns content of the message tip data.
    \sa         setContent
*/
const QString& QCtmMessageTipData::content() const
{
    return m_impl->content;
}

/*!
    \brief      Sets date \a time of the message tip data.
    \sa         dateTime()
*/
void QCtmMessageTipData::setDateTime(const QDateTime& time)
{
    m_impl->time = time;
}

/*!
    \brief      Returns date time of the message tip data.
    \sa         setDateTime
*/
const QDateTime& QCtmMessageTipData::dateTime() const
{
    return m_impl->time;
}
