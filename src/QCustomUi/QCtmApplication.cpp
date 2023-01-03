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

#include "QCtmApplication.h"
#include "Private/QCtmFramelessDelegate_win.h"
#include "QCtmStyleSheet.h"

#include <QAbstractNativeEventFilter>
#include <QLayout>
#include <QLocalServer>
#include <QLocalSocket>
#include <QWidget>
#include <QWindow>

#ifdef Q_OS_WIN
#include <qt_windows.h>

class WindowsEventFilter : public QAbstractNativeEventFilter
{
public:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override
#else
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override
#endif
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if (msg->message == WM_NCHITTEST)
        {
            auto w = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
            if (w && w->window() && w->window() != w)
            {
                auto d = w->window()->findChild<QCtmWinFramelessDelegate*>();
                if (d && d->onNCTitTest(msg, result))
                {
                    *result = HTTRANSPARENT;
                    return true;
                }
                return false;
            }
        }
        return false;
    }
};

#endif // Q_OS_WIN

struct QCtmApplication::Impl
{
    QLocalServer server;
    QLocalSocket client;
    QString serverName;
};

/*!
    \class      QCtmApplication
    \brief      QApplication 的扩展，设置了默认样式和单实例运行的接口.
    \inherits   QApplication
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmApplication.h
*/

/*!
    \brief      构造函数 \a argc, \a argv, \a f, \a defaultStyle
*/
QCtmApplication::QCtmApplication(int& argc, char** argv, int f /*= ApplicationFlags*/, bool defaultStyle)
    : QApplication(argc, argv, f), m_impl(std::make_unique<Impl>())
{
    setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    if (defaultStyle)
    {
        setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
    }
#ifdef Q_OS_WIN
    installNativeEventFilter(new WindowsEventFilter);
#endif // Q_OS_WIN
}

/*!
    \brief      析构函数.
*/
QCtmApplication::~QCtmApplication() {}

/*!
    \brief      检测并返回其他程序是否持有相同的 \a key.
*/
bool QCtmApplication::checkOtherProcess(const QString& key)
{
    if (key.isEmpty())
        return false;

    connect(&m_impl->server,
            &QLocalServer::newConnection,
            this,
            [this]()
            {
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
