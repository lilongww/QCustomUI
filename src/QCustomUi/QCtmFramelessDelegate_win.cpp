#include "QCtmFramelessDelegate_win.h"
#include "QCtmTitleBar.h"

#include <QFocusEvent>
#include <QApplication>
#include <QLayout>
#include <QPainter>
#include <QPen>
#include <QDebug>

#ifdef Q_OS_WIN

#include <QWinFunctions.h>

#include <windows.h>
#include <windowsx.h>

constexpr long AeroBorderlessFlag = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long BasicBorderlessFlag = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long AeroBorderlessFixedFlag = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
constexpr long BasicBorderlessFixedFlag = WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

struct QCtmWinFramelessDelegate::Impl
{
	QWidgetList moveBars;
	QWidget* parent{ nullptr };
	bool firstShow{ true };
};

QCtmWinFramelessDelegate::QCtmWinFramelessDelegate(QWidget* parent, const QWidgetList& moveBars)
	: QObject(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->parent = parent;
	parent->setWindowFlags(parent->windowFlags() | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);
	parent->installEventFilter(this);

	m_impl->moveBars = moveBars;
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
	if(!m_impl->moveBars.contains(w))
		m_impl->moveBars.append(w);
}

void QCtmWinFramelessDelegate::removeMoveBar(QWidget* w)
{
	m_impl->moveBars.removeOne(w);
}

bool QCtmWinFramelessDelegate::nativeEvent(const QByteArray& eventType
	, void* message
	, long* &result)
{
	MSG* msg = static_cast<MSG*>(message);
	switch (msg->message) {
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
		*result = 0;
		return true;
	}
	break;
	case WM_SYSCOMMAND:
	{
		if (msg->wParam == SC_KEYMENU && msg->lParam == VK_SPACE) {

			auto hwnd = (HWND)m_impl->parent->winId();
			HMENU menu = GetSystemMenu(hwnd, false);

			if (menu) {
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
				SetMenuItemInfo(menu, SC_SIZE, TRUE, &mii);
				SetMenuItemInfo(menu, SC_MOVE, TRUE, &mii);

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
					SetMenuItemInfo(menu, SC_RESTORE, FALSE, &mii);
					SetMenuDefaultItem(menu, SC_CLOSE, FALSE);
					break;
				}

				auto devicePixelRatio = qApp->devicePixelRatio();
				auto globalPos = m_impl->parent->pos();
				globalPos.rx() *= devicePixelRatio;
				globalPos.ry() *= devicePixelRatio;
				BOOL cmd = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD
					, globalPos.x()
					, globalPos.y()
					, hwnd
					, nullptr);
				if (cmd) PostMessage(hwnd, WM_SYSCOMMAND, cmd, 0);

				*result = 0;
				return true;
			}
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
		QPoint globalPos = QCursor::pos();
		int x = globalPos.x();
		int y = globalPos.y();
		auto borderX = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
		auto borderY = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

		if (m_impl->parent->isMaximized())
		{
			borderX = 0;
			borderY = 0;
		}

		RECT winrect;
		GetWindowRect((HWND)(m_impl->parent->winId()), &winrect);

		auto localPos = m_impl->parent->mapFromGlobal(globalPos);

		if (x >= winrect.left && x <= winrect.left + borderX) {
			if (y >= winrect.top && y <= winrect.top + borderY) {
				*result = HTTOPLEFT;
				return true;
			}
			if (y > winrect.top + borderY && y < winrect.bottom - borderY) {
				*result = HTLEFT;
				return true;
			}
			if (y >= winrect.bottom - borderY && y <= winrect.bottom) {
				*result = HTBOTTOMLEFT;
				return true;
			}
		}
		else if (x > winrect.left + borderX && x < winrect.right - borderX) {
			if (y >= winrect.top && y <= winrect.top + borderY) {
				*result = HTTOP;
				return true;
			}
			if (y > winrect.top + borderY && y < winrect.top + borderY) {
				*result = HTCAPTION;
				return true;
			}
			if (y >= winrect.bottom - borderY && y <= winrect.bottom) {
				*result = HTBOTTOM;
				return true;
			}
		}
		else if (x >= winrect.right - borderX && x <= winrect.right) {
			if (y >= winrect.top && y <= winrect.top + borderY) {
				*result = HTTOPRIGHT;
				return true;
			}
			if (y > winrect.top + borderY && y < winrect.bottom - borderY) {
				*result = HTRIGHT;
				return true;
			}
			if (y >= winrect.bottom - borderY && y <= winrect.bottom) {
				*result = HTBOTTOMRIGHT;
				return true;
			}
		}
		else {
			*result = HTNOWHERE;
			return true;
		}


		for (auto w : m_impl->moveBars)
		{
			auto child = m_impl->parent->childAt(localPos);

			if(!child)
				continue;

			if (child == w)
			{
				auto pos = w->mapFrom(m_impl->parent, localPos);
				auto title = qobject_cast<QCtmTitleBar*>(w);
				if (title)
				{
					if (title->showIcon() && title->doIconRect().contains(pos))
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
					&& w->metaObject()->className() != "QWidget"
					&& w->metaObject()->className() != "QLabel")
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
		if (!QtWin::isCompositionEnabled()) {
			*result = 1;
			return true;
		}
	}
	break;
	case WM_DWMCOMPOSITIONCHANGED:
	{
		if (m_impl->parent->isMaximized()) {
			if (QtWin::isCompositionEnabled()) {
				auto margin = 8 / m_impl->parent->devicePixelRatioF();
				m_impl->parent->layout()->setContentsMargins(QMargins(margin, margin, margin, margin));
			}
			else {
				m_impl->parent->layout()->setContentsMargins(QMargins(0, 0, 0, 0));
			}
		}
		else {
			m_impl->parent->layout()->setContentsMargins(QMargins(0, 0, 0, 0));
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
			if (m_impl->parent->isMaximized() && QtWin::isCompositionEnabled()) {
				auto margin = 8 / m_impl->parent->devicePixelRatioF();
				m_impl->parent->layout()->setContentsMargins(QMargins(margin, margin, margin, margin));
				m_impl->parent->layout()->update();
			}
			else {
				m_impl->parent->layout()->setContentsMargins(QMargins(0, 0, 0, 0));
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
	if (QtWin::isCompositionEnabled())
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

	if (QtWin::isCompositionEnabled())
	{
		QtWin::extendFrameIntoClientArea(m_impl->parent, 1, 1, 1, 1);
	}

	SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

}

#endif