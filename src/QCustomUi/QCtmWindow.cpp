/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2022 LiLong                                              **
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

#include "QCtmWindow.h"
#include "Private/QCtmFramelessDelegate_p.h"
#include "Private/QCtmFramelessDelegate_win.h"
#include "QCtmNavigationBar.h"
#include "QCtmTitleBar.h"
#include "ui_QCtmWindow.h"

#include <QEvent>
#include <QLabel>
#include <QResizeEvent>
#include <QSizeGrip>
#include <QStatusBar>
#include <QVBoxLayout>

/*!
     \class     QCtmWindow
     \brief     顶层自定义无边框窗口.
     \inherits  QWidget
     \ingroup   QCustomUi
     \inmodule  QCustomUi
     \inheaderfile QCtmWindow.h
*/

struct QCtmWindow::Impl
{
    QCtmTitleBar* title { nullptr };
    QCtmNavigationBar* navigationMenuBar { nullptr };
    QPointer<QStatusBar> statusBar;
    QWidget* content { nullptr };

#ifdef Q_OS_WIN
    QCtmWinFramelessDelegate* delegate { nullptr };
#else
    QCtmFramelessDelegate* delegate { nullptr };
#endif
};

/*!
    \brief      构造函数 \a parent.
*/
QCtmWindow::QCtmWindow(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>()), ui(new Ui::QCtmWindow)
{
    m_impl->title = new QCtmTitleBar(this);
    m_impl->title->installEventFilter(this);
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_impl->content = new QWidget(this);
    ui->titleLayout->addWidget(m_impl->title);
    ui->contentLayout->addWidget(m_impl->content);
    m_impl->content->installEventFilter(this);

#ifdef Q_OS_WIN
    m_impl->delegate = new QCtmWinFramelessDelegate(this, m_impl->title);
#else
    m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
    m_impl->content->setAutoFillBackground(true);
#endif
}

/*!
    \brief      析构函数.
*/
QCtmWindow::~QCtmWindow() { delete ui; }

/*!
    \brief      设置状态栏 \a statusBar, 当设置 statusBar 为 nullptr 时将删除状态栏.
    \sa         statusBar
*/
void QCtmWindow::setStatusBar(QStatusBar* statusBar)
{
    if (m_impl->statusBar == statusBar)
        return;
    if (m_impl->statusBar)
    {
        m_impl->statusBar->hide();
        m_impl->statusBar->deleteLater();
    }
    m_impl->statusBar = statusBar;
    if (m_impl->statusBar)
        ui->statusBarLayout->addWidget(statusBar);
}

/*!
    \brief      返回状态栏，如果状态栏对象不存在，QCtmWindow会自动创建一个状态栏对象返回.
    \sa         setStatusBar
*/
QStatusBar* QCtmWindow::statusBar() const
{
    auto statusBar = m_impl->statusBar;
    if (!statusBar)
    {
        auto self = const_cast<QCtmWindow*>(this);
        statusBar = new QStatusBar(self);
        statusBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        self->setStatusBar(statusBar);
    }
    return statusBar;
}

/*!
    \brief      设置菜单栏 \a menuBar.
    \sa         menuBar, removeMenuBar
*/
void QCtmWindow::setMenuBar(QMenuBar* menuBar)
{
    if (m_impl->title)
    {
        m_impl->title->setMenuBar(menuBar);
    }
}

/*!
    \brief      返回菜单栏.
    \sa         setMenuBar, removeMenuBar
*/
QMenuBar* QCtmWindow::menuBar() const
{
    if (m_impl->title)
        return m_impl->title->menuBar();
    return nullptr;
}

/*!
    \brief      移除菜单栏.
    \sa         setMenuBar, menuBar
*/
void QCtmWindow::removeMenuBar()
{
    if (m_impl->title)
    {
        m_impl->title->removeMenuBar();
    }
}

/*!
    \brief      设置导航栏 \a bar.
    \sa         navigationBar, removeNavigationBar
*/
void QCtmWindow::setNavigationBar(QCtmNavigationBar* bar)
{
    removeNavigationBar();
    m_impl->navigationMenuBar = bar;
    ui->menuBarLayout->addWidget(bar);
    if (!m_impl->title)
        m_impl->delegate->addMoveBar(bar);
    else if (!m_impl->title->isVisible())
        m_impl->delegate->addMoveBar(bar);
}

/*!
    \brief      返回导航栏.
    \sa         setNavigationBar, removeNavigationBar
*/
QCtmNavigationBar* QCtmWindow::navigationBar() const { return m_impl->navigationMenuBar; }

/*!
    \brief      移除导航栏.
    \sa         setNavigationBar, navigationBar
*/
void QCtmWindow::removeNavigationBar()
{
    if (m_impl->navigationMenuBar)
    {
        m_impl->delegate->removeMoveBar(m_impl->navigationMenuBar);
        delete m_impl->navigationMenuBar;
        m_impl->navigationMenuBar = nullptr;
    }
}

/*!
    \brief      设置标题栏 \a titleBar.
    \sa         titleBar(), removeTitleBar()
*/
void QCtmWindow::setTitleBar(QCtmTitleBar* titleBar)
{
    removeTitleBar();
    m_impl->title = titleBar;
    ui->titleLayout->addWidget(titleBar);
    titleBar->installEventFilter(this);
    if (m_impl->navigationMenuBar)
    {
        m_impl->delegate->removeMoveBar(m_impl->navigationMenuBar);
    }
}

/*!
    \brief      返回标题栏.
    \sa         setTitleBar, removeTitleBar()
*/
QCtmTitleBar* QCtmWindow::titleBar() const { return m_impl->title; }

/*!
    \brief      移除标题栏.
    \sa         setTitleBar, titleBar
*/
void QCtmWindow::removeTitleBar()
{
    if (m_impl->title)
    {
        m_impl->delegate->removeMoveBar(m_impl->title);
        delete m_impl->title;
        m_impl->title = nullptr;
    }
}

/*!
    \brief      设置中央窗口 \a widget.
    \sa         centralWidget
*/
void QCtmWindow::setCentralWidget(QWidget* widget)
{
    delete m_impl->content;
    m_impl->content = widget;
    widget->setAutoFillBackground(true);
    ui->contentLayout->addWidget(widget);
    setWindowTitle(widget->windowTitle());
}

/*!
    \brief      返回中央窗口.
    \sa         setCentralWidget
*/
QWidget* QCtmWindow::centralWidget() const { return m_impl->content; }

/*!
    \brief      添加移动窗口 \a moveBar, 移动窗口可通过鼠标拖动来拖动窗口.
    \note       移动窗口必须本身就是本窗口的子窗口.
    \sa         removeMoveBar
*/
void QCtmWindow::addMoveBar(QWidget* moveBar) { m_impl->delegate->addMoveBar(moveBar); }

/*!
    \brief      移除移动窗口 \a moveBar.
    \sa         addMoveBar
*/
void QCtmWindow::removeMoveBar(QWidget* moveBar) { m_impl->delegate->removeMoveBar(moveBar); }

#ifndef Q_OS_WIN

/*!
    \if         !defined(Q_OS_WIN)
    \fn         void QCtmWindow::setShadowless(bool flag)
    \brief      Set the window is shadowless by \a flag.
    \sa         shadowless
    \else
    \internal
    \endif
*/
void QCtmWindow::setShadowless(bool flag) { m_impl->delegate->setShadowless(flag); }

/*!
    \if         !defined(Q_OS_WIN)
    \fn         bool QCtmWindow::shadowless() const
    \brief      Get the window is shadowless.
    \sa         setShadowless
    \else
    \internal
    \endif
*/
bool QCtmWindow::shadowless() const { return m_impl->delegate->shadowless(); }
#endif

/*!
    \reimp
*/
bool QCtmWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->title)
    {
        if (event->type() == QEvent::Close || event->type() == QEvent::Hide)
        {
            m_impl->delegate->removeMoveBar(m_impl->title);
            if (m_impl->navigationMenuBar)
            {
                m_impl->delegate->addMoveBar(m_impl->navigationMenuBar);
            }
        }
        else if (event->type() == QEvent::Show)
        {
            m_impl->delegate->addMoveBar(m_impl->title);
            if (m_impl->navigationMenuBar)
            {
                m_impl->delegate->removeMoveBar(m_impl->navigationMenuBar);
            }
        }
    }
    else if (watched == m_impl->content && event->type() == QEvent::WindowTitleChange)
    {
        setWindowTitle(m_impl->content->windowTitle());
    }
    return QWidget::eventFilter(watched, event);
}

/*!
    \reimp
*/
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QCtmWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
#else
bool QCtmWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#endif
{
#ifdef Q_OS_WIN
    if (!m_impl->delegate)
        return QWidget::nativeEvent(eventType, message, result);
    if (!m_impl->delegate->nativeEvent(eventType, message, result))
        return QWidget::nativeEvent(eventType, message, result);
    else
        return true;
#else
    return QWidget::nativeEvent(eventType, message, result);
#endif
}
