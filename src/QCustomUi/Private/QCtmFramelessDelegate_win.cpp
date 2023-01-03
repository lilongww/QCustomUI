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

#include "QCtmFramelessDelegate_win.h"
#include "QCtmTitleBar.h"

#include <QApplication>
#include <QBackingStore>
#include <QDebug>
#include <QFocusEvent>
#include <QLayout>
#include <QPainter>
#include <QPen>
#include <QScopeGuard>
#include <QScreen>
#include <QWindow>

#ifdef Q_OS_WIN
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtWin>
#endif
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

constexpr long BorderlessFlag = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

inline bool isCompositionEnabled()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QtWin::isCompositionEnabled();
#else
    BOOL enabled = FALSE;
    DwmIsCompositionEnabled(&enabled);
    return enabled;
#endif
}

inline void extendFrameIntoClientArea(QWindow* window, int left, int top, int right, int bottom)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QtWin::extendFrameIntoClientArea(window, left, top, right, bottom);
#else
    MARGINS margins = { left, right, top, bottom };
    DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(window->winId()), &margins);
#endif
}

inline std::optional<QRect> getScreenNativeWorkRect(HWND hwnd)
{
    auto monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
    if (!monitor)
        return std::nullopt;
    MONITORINFO info;
    ::ZeroMemory(&info, sizeof(info));
    info.cbSize = sizeof(info);
    GetMonitorInfo(monitor, &info);
    return QRect(info.rcWork.left, info.rcWork.top, info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top);
}

#if 0
inline std::optional<QRect> getScreenNativeRect(HWND hwnd)
{
    auto monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
    if (!monitor)
        return std::nullopt;
    MONITORINFO info;
    ::ZeroMemory(&info, sizeof(info));
    info.cbSize = sizeof(info);
    GetMonitorInfo(monitor, &info);
    return QRect(
        info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top);
}
#endif

struct QCtmWinFramelessDelegate::Impl
{
    QWidgetList moveBars;
    QWidget* parent { nullptr };
    bool firstShow { true };
    WINDOWPLACEMENT wndPlaceMent;
    std::optional<QRect> workRect;
    HMONITOR monitor { nullptr };
    inline double dpiScale(double value) { return value / parent->devicePixelRatioF(); }
    inline double unDpiScale(double value) { return value * parent->devicePixelRatioF(); }
    inline void checkMonitorChanged()
    {
        auto hwnd       = reinterpret_cast<HWND>(parent->winId());
        auto newMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
        if (newMonitor != monitor)
        {
            monitor = newMonitor;
            if (auto ret = getScreenNativeWorkRect(hwnd); ret)
                workRect = ret;
        }
    }
};

QCtmWinFramelessDelegate::QCtmWinFramelessDelegate(QWidget* parent, const QWidgetList& moveBars)
    : QObject(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->parent = parent;
    parent->installEventFilter(this);
    m_impl->moveBars = moveBars;
    m_impl->workRect = getScreenNativeWorkRect(reinterpret_cast<HWND>(parent->winId()));
    connect(parent->screen(),
            &QScreen::availableGeometryChanged,
            this,
            [=](const QRect&) { m_impl->workRect = getScreenNativeWorkRect(reinterpret_cast<HWND>(parent->winId())); });
}

QCtmWinFramelessDelegate::QCtmWinFramelessDelegate(QWidget* parent, QWidget* title)
    : QCtmWinFramelessDelegate(parent, QWidgetList { title })
{
}

QCtmWinFramelessDelegate::~QCtmWinFramelessDelegate() {}

void QCtmWinFramelessDelegate::addMoveBar(QWidget* w)
{
    if (!isCompositionEnabled() && qobject_cast<QCtmTitleBar*>(w)) // for win7 解决出现默认关闭按钮等问题.
    {
        w->winId();
    }
    if (!m_impl->moveBars.contains(w))
        m_impl->moveBars.append(w);
}

void QCtmWinFramelessDelegate::removeMoveBar(QWidget* w) { m_impl->moveBars.removeOne(w); }

inline void debugPos(unsigned i)
{
#define Pair(a) std::pair<int, const char*>(a, #a)
    static std::map<int, const char*> values { Pair(SWP_NOSIZE),       Pair(SWP_NOSIZE),        Pair(SWP_NOMOVE),
                                               Pair(SWP_NOZORDER),     Pair(SWP_NOREDRAW),      Pair(SWP_NOACTIVATE),
                                               Pair(SWP_FRAMECHANGED), Pair(SWP_SHOWWINDOW),    Pair(SWP_HIDEWINDOW),
                                               Pair(SWP_NOCOPYBITS),   Pair(SWP_NOOWNERZORDER), Pair(SWP_NOSENDCHANGING),
                                               Pair(SWP_DEFERERASE),   Pair(SWP_ASYNCWINDOWPOS) };
    std::for_each(values.begin(),
                  values.end(),
                  [i](const auto& value)
                  {
                      if (value.first & i)
                      {
                          qDebug() << value.second;
                      }
                  });
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QCtmWinFramelessDelegate::nativeEvent([[maybe_unused]] const QByteArray& eventType, void* message, long* result)
#else
bool QCtmWinFramelessDelegate::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#endif
{
    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message)
    {
    case WM_NCMOUSEMOVE:
        break;
    case WM_NCPAINT:
        if (!isCompositionEnabled())
        {
            *result = 0;
            return true;
        }
        else
            break;
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
                if (IsZoomed(msg->hwnd))
                {
                    m_impl->checkMonitorChanged();
                    auto rc = m_impl->workRect;
                    if (!rc)
                        return false;
                    if (auto ret = DefWindowProcW(msg->hwnd, WM_NCCALCSIZE, msg->wParam, msg->lParam); ret)
                    {
                        *result = ret;
                        return true;
                    }
                    NCCALCSIZE_PARAMS* ncParam = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
                    ncParam->rgrc[0].top       = rc->top();
                    ncParam->rgrc[0].bottom    = rc->bottom() + 1;
                    *result                    = 0;
                    return true;
                }
                else // 优化窗口大小调整闪烁问题
                {
                    const auto clientRect = reinterpret_cast<LPRECT>(msg->lParam);
                    const auto before     = *clientRect;
                    if (auto ret = DefWindowProcW(msg->hwnd, WM_NCCALCSIZE, msg->wParam, msg->lParam); ret)
                    {
                        *result = ret;
                        return true;
                    }
                    if (!isCompositionEnabled())
                    {
                        *clientRect = before;
                    }
                    else
                    {
                        clientRect->top    = before.top;
                        clientRect->left   = before.left;
                        clientRect->right  = before.right;
                        clientRect->bottom = before.bottom + 1;
                    }
                }
            }
            *result = !msg->wParam ? 0 : WVR_REDRAW;
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
            if (msg->wParam == VK_SPACE)
            {
                *result = 0;
                return true;
            }
        }
        break;
    case WM_NCHITTEST:
        return onNCTitTest(msg, result);
    case WM_NCACTIVATE:
        {
            if (!isCompositionEnabled())
            {
                *result = 1;
                return true;
            }
        }
        break;
    case WM_NCLBUTTONDBLCLK:
        {
            if (m_impl->parent->maximumSize() == QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX) &&
                m_impl->parent->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
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
        // qDebug() << event->type();
        if (event->type() == QEvent::WindowStateChange)
        {
            int i = 0;
        }
        else if (event->type() == QEvent::Show)
        {
            if (m_impl->firstShow)
            {
                m_impl->firstShow = false;
                setWindowLong();
            }
            m_impl->parent->setAttribute(Qt::WA_Resized);
            if (!m_impl->firstShow && m_impl->parent->isMaximized())
            {
                QMetaObject::invokeMethod(
                    this,
                    [=]()
                    {
                        if (m_impl->wndPlaceMent.showCmd != SW_SHOWMINIMIZED)
                            SetWindowPlacement((HWND)m_impl->parent->winId(), &m_impl->wndPlaceMent);
                    },
                    Qt::QueuedConnection);
            }
        }
        else if (event->type() == QEvent::Hide)
        {
            m_impl->wndPlaceMent.length = sizeof(m_impl->wndPlaceMent);
            GetWindowPlacement((HWND)m_impl->parent->winId(), &m_impl->wndPlaceMent);
        }
        else if (event->type() == QEvent::Close)
        {
            if (!m_impl->parent->windowFlags().testFlag(Qt::Dialog))
            {
                m_impl->wndPlaceMent.length = sizeof(m_impl->wndPlaceMent);
                GetWindowPlacement((HWND)m_impl->parent->winId(), &m_impl->wndPlaceMent);
            }
        }
        else if (event->type() == QEvent::WinIdChange)
        {
            m_impl->firstShow = true;
        }
        else if (event->type() == QEvent::ScreenChangeInternal)
        {
            SetWindowPos(reinterpret_cast<HWND>(m_impl->parent->winId()), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
            return false;
        }
    }
    return false;
}

void QCtmWinFramelessDelegate::setWindowLong()
{
    auto hwnd  = reinterpret_cast<HWND>(m_impl->parent->winId());
    long style = BorderlessFlag;

    if (!m_impl->parent->windowFlags().testFlag(Qt::WindowMinimizeButtonHint) ||
        m_impl->parent->maximumSize() != QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX))
        style &= ~WS_MINIMIZEBOX;
    if (!m_impl->parent->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
        style &= ~WS_MAXIMIZEBOX;
    SetWindowLongPtr(hwnd, GWL_STYLE, style);

    if (isCompositionEnabled())
    {
        extendFrameIntoClientArea(m_impl->parent->windowHandle(), 1, 0, 0, 0);
    }
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(
        hwnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void QCtmWinFramelessDelegate::showSystemMenu(const QPoint& pos)
{
    auto hwnd  = (HWND)m_impl->parent->winId();
    HMENU menu = GetSystemMenu(hwnd, false);

    if (menu)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask  = MIIM_STATE;
        mii.fType  = 0;

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
        auto globalPos        = pos;
        globalPos.rx() *= devicePixelRatio;
        globalPos.ry() *= devicePixelRatio;
        BOOL cmd = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, globalPos.x(), globalPos.y(), hwnd, nullptr);
        if (cmd)
            PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QCtmWinFramelessDelegate::onNCTitTest(MSG* msg, long* result)
#else
bool QCtmWinFramelessDelegate::onNCTitTest(MSG* msg, qintptr* result)
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto [x, y] = QCursor::pos();
#else
    auto p = QCursor::pos();
    auto x = p.x();
    auto y = p.y();
#endif

    auto borderX = GetSystemMetrics(SM_CXPADDEDBORDER);
    auto borderY = GetSystemMetrics(SM_CXPADDEDBORDER);

    bool maxSized = m_impl->parent->isMaximized() || m_impl->parent->isFullScreen();
    if (maxSized)
    {
        borderX = 0;
        borderY = 0;
    }
    if (m_impl->parent->minimumSize() != m_impl->parent->maximumSize())
    {
        auto rect = m_impl->parent->geometry();

        if (!maxSized && x >= rect.left() && x <= rect.left() + borderX)
        {
            if (y >= rect.top() && y <= rect.top() + borderY)
            {
                *result = HTTOPLEFT;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.bottom() - borderY)
            {
                *result = HTLEFT;
                return true;
            }
            if (y >= rect.bottom() - borderY && y <= rect.bottom())
            {
                *result = HTBOTTOMLEFT;
                return true;
            }
        }
        else if (x > rect.left() + borderX && x < rect.right() - borderX)
        {
            if (!maxSized && y >= rect.top() && y <= rect.top() + borderY)
            {
                *result = HTTOP;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.top() + borderY)
            {
                *result = HTCAPTION;
                return true;
            }
            if (!maxSized && y >= rect.bottom() - borderY && y <= rect.bottom())
            {
                *result = HTBOTTOM;
                return true;
            }
        }
        else if (!maxSized && x >= rect.right() - borderX && x <= rect.right())
        {
            if (y >= rect.top() && y <= rect.top() + borderY)
            {
                *result = HTTOPRIGHT;
                return true;
            }
            if (y > rect.top() + borderY && y < rect.bottom() - borderY)
            {
                *result = HTRIGHT;
                return true;
            }
            if (y >= rect.bottom() - borderY && y <= rect.bottom())
            {
                *result = HTBOTTOMRIGHT;
                return true;
            }
        }
        else if (!maxSized)
        {
            *result = HTNOWHERE;
            return true;
        }
    }

    auto localPos = m_impl->parent->mapFromGlobal(QPoint(x, y));
    for (auto w : m_impl->moveBars)
    {
        auto child = m_impl->parent->childAt(localPos);

        if (!child)
            continue;

        if (child == w)
        {
            auto pos   = w->mapFrom(m_impl->parent, localPos);
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
            auto it = std::find_if(w->children().begin(),
                                   w->children().end(),
                                   [&](const auto& obj)
                                   {
                                       if (obj->isWidgetType())
                                       {
                                           return obj == child;
                                       }
                                       return false;
                                   });

            if (it != w->children().end() && w->metaObject()->className() != QString("QWidget") &&
                w->metaObject()->className() != QString("QLabel"))
            {
                // if ((*it)->property("qcustomui_maximumSizeButton").isValid())
                //{
                //     *result = HTMAXBUTTON;
                //     return false;
                // }
                *result = HTCLIENT;
                return false;
            }
        }
    }
    return false;
}
#endif
