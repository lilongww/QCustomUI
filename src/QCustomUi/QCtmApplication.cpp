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

#include "QCtmApplication.h"
#include "QCtmStyleSheet.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QWidget>
#include <QWindow>

struct QCtmApplication::Impl
{
    QLocalServer server;
    QLocalSocket client;
    QString serverName;
};

/*!
    \class      QCtmApplication
    \brief      QCtmApplication provide a function to set the application singleton run, and set the default style sheet.
    \inherits   QApplication
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a application with \a argc, \a argv and \a f.
*/
QCtmApplication::QCtmApplication(int& argc, char** argv, int f/*= ApplicationFlags*/)
    : QApplication(argc, argv, f)
    , m_impl(std::make_unique<Impl>())
{
    setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
}

/*!
    \brief      Destroys the application.
*/
QCtmApplication::~QCtmApplication()
{
}

/*!
    \brief      Check to see if any other processes start using the same \a key.
                Returns true if other processes start with the \a key.
*/
bool QCtmApplication::checkOtherProcess(const QString& key)
{
    if (key.isEmpty())
        return false;

    connect(&m_impl->server, &QLocalServer::newConnection, this, [this]() {
        const auto& ws = this->topLevelWindows();
        if (!ws.isEmpty())
        {
            for (const auto& w : ws)
            {
                if (w->isVisible())
                {
                    w->show();
                }
                else
                    w->raise();
            }
        }
        });

    m_impl->client.connectToServer(key);
    if (m_impl->client.waitForConnected(100))
        return true;

    if (!m_impl->server.listen(key))
    {
        return true;
    }
    return false;
}
