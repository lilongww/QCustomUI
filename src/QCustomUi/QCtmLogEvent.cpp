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

#include "QCtmLogEvent.h"
#include "QCtmLogData.h"

struct QCtmLogEvent::Impl
{
    QCtmLogDataPtr log;
};

/*!
    \class      QCtmLogEvent
    \brief      QCtmLogEvent provide a custom event to transfer log message datas.
    \inherits   QEvent
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a log event with \a log message data.
*/
QCtmLogEvent::QCtmLogEvent(QCtmLogDataPtr log)
    : QEvent((QEvent::Type)QCtmLog)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->log = log;
}

/*!
    \brief      Destroys the log event.
*/
QCtmLogEvent::~QCtmLogEvent()
{

}

/*!
    \brief      Returns the log message data.
*/
QCtmLogDataPtr QCtmLogEvent::log() const
{
    return m_impl->log;
}
