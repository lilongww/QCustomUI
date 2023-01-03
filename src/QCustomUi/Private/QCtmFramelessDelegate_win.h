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

#include <QWidget>

#include <functional>
#include <memory>

#ifdef Q_OS_WIN

using MSG = struct tagMSG;
class QCtmWinFramelessDelegate : public QObject
{
    Q_OBJECT

public:
    QCtmWinFramelessDelegate(QWidget* parent, const QWidgetList& moveBars);
    QCtmWinFramelessDelegate(QWidget* parent, QWidget* title);
    ~QCtmWinFramelessDelegate();

    void addMoveBar(QWidget* w);
    void removeMoveBar(QWidget* w);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#endif
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void setWindowLong();
    void showSystemMenu(const QPoint& pos);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool onNCTitTest(MSG* msg, long* result);
#else
    bool onNCTitTest(MSG* msg, qintptr* result);
#endif
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class WindowsEventFilter;
};
#endif
