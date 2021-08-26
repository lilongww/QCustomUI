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

#include "QCtmFramelessDelegate_win.h"
#include "QCtmTitleBar.h"

#include <QFocusEvent>
#include <QApplication>
#include <QLayout>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QWindow>
#include <QBackingStore>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

constexpr long AeroBorderlessFlag = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long BasicBorderlessFlag = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long AeroBorderlessFixedFlag = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long BasicBorderlessFixedFlag = WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

inline bool isCompositionEnabled()
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return QtWin::isCompositionEnabled();
#else
    BOOL enabled = FALSE;
    DwmIsCompositionEnabled(&enabled);
    return enabled;
#endif
}

inline void extendFrameIntoClientArea(QWindow* window, int left, int top, int right, int bottom)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QtWin::extendFrameIntoClientArea(window, left, top, right, bottom);
#else
    MARGINS margins = { left, right, top, bottom };
    DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(window->winId()), &margins);
#endif
}

struct QCtmWinFramelessDelegate::Impl
{
    QWidgetList moveBars;
    QWidget* parent{ nullptr };
    bool firstShow{ true };

    inline void setNoMargins()
    {
        parent->layout()->setContentsMargins(QMargins(0, 0, 0, 0));
    };
};

QCtmWinFramelessDelegate::QCtmWinFramelessDelegate(QWidget* parent, const QWidgetList& moveBars)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->parent = parent;
    parent->setWindowFlags(parent->windowFlags()
        | Qt::FramelessWindowHint
        | Qt::WindowMinMaxButtonsHint
        | Qt::WindowCloseButtonHint
        | Qt::WindowSystemMenuHint);
    parent->installEventFilter(this);
    m_impl->moveBars = moveBars;
    m_impl->setNoMargins();
    m_impl->parent->setProperty("qcustomui-window", true);
}

QCtmWinFramelessDelegate::QCtmWinFramelessDelegate(QWidget* parent, QWidget* title)
    : QCtmWinFramelessDelegate(parent, QWidgetList{ title })
{
}

QCtmWinFramelessDelegate::~QCtmWinFramelessDelegate()
{
}

void QCtmWinFramelessDelegate::addMoveBar(QWidget* w)
{
    if (!m_impl->moveBars.contains(w))
        m_impl->moveBars.append(w);
}

void QCtmWinFramelessDelegate::removeMoveBar(QWidget* w)
{
    m_impl->moveBars.removeOne(w);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool QCtmWinFramelessDelegate::nativeEvent([[maybe_unused]] const QByteArray& eventType
    , void* message
    , long* result)
#else
bool QCtmWinFramelessDelegate::nativeEvent(const QByteArray& eventType
    , void* message
    , qintptr* result)
#endif
{
    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message)
    {
    case WM_SETFOCUS:
    {
        Qt::FocusReason reason;
        if (::GetKeyState(VK_LBUTTON) < 0 || ::GetKeyState(VK_RBUTTON) < 0)
            reason = Qt::MouseFocusReason;
        else if (::GetKeyState(VK_SHIFT) < 0)
            reason = Qt::BacktabFocusReason;
        else
            reason = Qt::TabFocusReason;

        QFocusEvent e(QEvent::FocusIn, reason);
        QApplication::sendEvent(m_impl->parent, &e);
    }
    break;
    case WM_NCCALCSIZE:
    {
        if (msg->wParam)
        {
            NCCALCSIZE_PARAMS* ncParam = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        }
        else
        {
            RECT* rect = reinterpret_cast<RECT*>(msg->lParam);
        }
        *result = 0;
        return true;
    }
    break;
    case WM_SYSCOMMAND:
    {
        if (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE)
        {
            showSystemMenu(m_impl->parent->pos());
            *result = 0;
            return true;
        }
    }
    break;
    case WM_SYSKEYDOWN:
    {
        if (msg->wParam == VK_SPACE) {
            *result = 0;
            return true;
    }
}
    break;
    case WM_NCHITTEST:
    {
        POINTS globalPos = MAKEPOINTS(msg->lParam);
        int x = globalPos.x;
        int y = globalPos.y;

        auto borderX = /*GetSystemMetrics(SM_CXFRAME) +*/ GetSystemMetrics(SM_CXPADDEDBORDER);
        auto borderY = /*GetSystemMetrics(SM_CYFRAME) +*/ GetSystemMetrics(SM_CXPADDEDBORDER);

        if (m_impl->parent->isMaximized())
        {
            borderX = 0;
            borderY = 0;
        }

        auto rect = m_impl->parent->frameGeometry();
        auto localPos = m_impl->parent->mapFromGlobal(QPoint(x, y));

        if (x >= rect.left() && x <= rect.left() + borderX) {
            if (y >= rect.top() && y <= rect.top() + borderY) {
                *result = HTTOPLEFT;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.bottom() - borderY) {
                *result = HTLEFT;
                return true;
            }
            if (y >= rect.bottom() - borderY && y <= rect.bottom()) {
                *result = HTBOTTOMLEFT;
                return true;
            }
        }
        else if (x > rect.left() + borderX && x < rect.right() - borderX) {
            if (y >= rect.top() && y <= rect.top() + borderY) {
                *result = HTTOP;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.top() + borderY) {
                *result = HTCAPTION;
                return true;
            }
            if (y >= rect.bottom() - borderY && y <= rect.bottom()) {
                *result = HTBOTTOM;
                return true;
            }
        }
        else if (x >= rect.right() - borderX && x <= rect.right()) {
            if (y >= rect.top() && y <= rect.top() + borderY) {
                *result = HTTOPRIGHT;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.bottom() - borderY) {
                *result = HTRIGHT;
                return true;
            }
            if (y >= rect.bottom() - borderY && y <= rect.bottom()) {
                *result = HTBOTTOMRIGHT;
                return true;
            }
        }
        else
        {
            *result = HTNOWHERE;
            return true;
        }

        for (auto w : m_impl->moveBars)
        {
            auto child = m_impl->parent->childAt(localPos);

            if (!child)
                continue;

            if (child == w)
            {
                auto pos = w->mapFrom(m_impl->parent, localPos);
                auto title = qobject_cast<QCtmTitleBar*>(w);
                if (title)
                {
                    if (title->iconIsVisible() && title->doIconRect().contains(pos))
                    {
                        *result = HTSYSMENU;
                        return true;
                    }
                }
                *result = HTCAPTION;
                return true;
            }
            else
            {
                auto it = std::find_if(w->children().begin(), w->children().end(), [&](const auto& obj)
                    {
                        if (obj->isWidgetType())
                        {
                            return obj == child;
                        }
                        return false;
                    });

                if (it != w->children().end()
                    && w->metaObject()->className() != QString("QWidget")
                    && w->metaObject()->className() != QString("QLabel"))
                {
                    *result = HTCLIENT;
                    return true;
                }
            }
        }
    }
    break;
    case WM_NCACTIVATE:
    {
        if (!isCompositionEnabled()) {
            *result = 1;
            return true;
        }
    }
    break;
    case WM_DWMCOMPOSITIONCHANGED:
    {
        if (m_impl->parent->isMaximized())
        {
            if (isCompositionEnabled())
            {
                auto margin = 8 / m_impl->parent->devicePixelRatioF();
                m_impl->parent->layout()->setContentsMargins(QMargins(margin, margin, margin, margin));
            }
            else
            {
                m_impl->setNoMargins();
            }
        }
        else
        {
            m_impl->setNoMargins();
        }
    }
    break;
    case WM_GETMINMAXINFO:
    {
        auto info = (MINMAXINFO*)msg->lParam;
        info->ptMinTrackSize.x = m_impl->parent->minimumWidth();
        info->ptMinTrackSize.y = m_impl->parent->minimumHeight();
        info->ptMaxTrackSize.x = m_impl->parent->maximumWidth();
        info->ptMaxTrackSize.y = m_impl->parent->maximumHeight();

        if (::MonitorFromWindow(::FindWindow(L"Shell_TrayWnd", nullptr), MONITOR_DEFAULTTONEAREST) ==
            ::MonitorFromWindow((HWND)(m_impl->parent->winId()), MONITOR_DEFAULTTONEAREST))
        {
            info->ptMaxPosition.x = 0;
            info->ptMaxPosition.y = 0;

            info->ptMaxSize.x = GetSystemMetrics(SM_CXFULLSCREEN) + GetSystemMetrics(SM_CXDLGFRAME);
            info->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION);
        }
        return true;
    }
    break;
    case WM_NCLBUTTONDBLCLK:
    {
        if (m_impl->parent->maximumSize() == QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)
            && m_impl->parent->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
            return false;
        else
            return true;
    }
    case WM_NCRBUTTONUP:
    {
        showSystemMenu(QCursor::pos());
        *result = 0;
        return true;
    }
    default:
        break;
}
    return false;
}

bool QCtmWinFramelessDelegate::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->parent)
    {
        if (event->type() == QEvent::WindowStateChange)
        {
            if (m_impl->parent->isMaximized() && isCompositionEnabled() && !m_impl->parent->windowFlags().testFlag(Qt::Tool))
            {
                auto margin = 8 / m_impl->parent->devicePixelRatioF();
                m_impl->parent->layout()->setContentsMargins(QMargins(margin, margin, margin, margin));
                m_impl->parent->layout()->update();
            }
            else
            {
                m_impl->setNoMargins();
                m_impl->parent->layout()->update();
                if (!m_impl->parent->underMouse())
                {
                    ::PostMessage((HWND)m_impl->parent->winId(), WM_NCMOUSELEAVE, 0, 0);
                }
            }
            m_impl->parent->repaint();
        }
        else if (event->type() == QEvent::Show)
        {
            if (m_impl->firstShow)
            {
                m_impl->firstShow = false;
                setWindowLong();
            }
        }
    }
    return false;
}

void QCtmWinFramelessDelegate::setWindowLong()
{
    auto hwnd = reinterpret_cast<HWND>(m_impl->parent->winId());

    long style;
    if (isCompositionEnabled())
    {
        if (m_impl->parent->maximumSize() == QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)
            && m_impl->parent->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
            style = AeroBorderlessFlag;
        else
            style = AeroBorderlessFixedFlag;
    }
    else
    {
        if (m_impl->parent->maximumSize() == QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)
            && m_impl->parent->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
            style = BasicBorderlessFlag;
        else
            style = BasicBorderlessFixedFlag;
    }
    SetWindowLongPtr(hwnd, GWL_STYLE, style);

    if (isCompositionEnabled())
    {
        extendFrameIntoClientArea(m_impl->parent->backingStore()->window(), 1, 1, 1, 1);
    }

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

}

void QCtmWinFramelessDelegate::showSystemMenu(const QPoint& pos)
{
    auto hwnd = (HWND)m_impl->parent->winId();
    HMENU menu = GetSystemMenu(hwnd, false);

    if (menu)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STATE;
        mii.fType = 0;

        mii.fState = MF_ENABLED;

        SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
        SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
        SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);

        // update the options
        mii.fState = MF_DISABLED;
        if (m_impl->parent->maximumSize() == m_impl->parent->minimumSize() || m_impl->parent->isMaximized())
            SetMenuItemInfo(menu, SC_SIZE, FALSE, &mii);
        else
            SetMenuItemInfo(menu, SC_SIZE, TRUE, &mii);

        if (!m_impl->parent->isMaximized())
            SetMenuItemInfo(menu, SC_MOVE, TRUE, &mii);
        else
            SetMenuItemInfo(menu, SC_MOVE, FALSE, &mii);

        mii.fState = MF_GRAYED;

        WINDOWPLACEMENT wp;
        GetWindowPlacement(hwnd, &wp);

        switch (wp.showCmd)
        {
        case SW_SHOWMAXIMIZED:
            SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
            SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
            break;
        case SW_SHOWMINIMIZED:
            SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);
            SetMenuDefaultItem(menu, SC_RESTORE, FALSE);
            break;
        case SW_SHOWNORMAL:
        {
            if (m_impl->parent->maximumSize() == m_impl->parent->minimumSize())
            {
                SetMenuItemInfo(menu, SC_MAXIMIZE, FALSE, &mii);
            }

            if (!m_impl->parent->windowFlags().testFlag(Qt::WindowMinimizeButtonHint))
            {
                SetMenuItemInfo(menu, SC_MINIMIZE, FALSE, &mii);
            }

            SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
            SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
            break;
        }
        }

        auto devicePixelRatio = qApp->devicePixelRatio();
        auto globalPos = pos;
        globalPos.rx() *= devicePixelRatio;
        globalPos.ry() *= devicePixelRatio;
        BOOL cmd = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD
            , globalPos.x()
            , globalPos.y()
            , hwnd
            , nullptr);
        if (cmd)
            PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);
    }
}

#endif
