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

#include "QCtmWindow.h"
#include "QCtmTitleBar.h"
#include "QCtmNavigationBar.h"
#include "ui_QCtmWindow.h"
#include "Private/QCtmFramelessDelegate_p.h"
#include "QCtmFramelessDelegate_win.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QStatusBar>
#include <QSizeGrip>
#include <QResizeEvent>

/*!
         \class QCtmWindow
         \brief This window is a base widget.
         \inherits QWidget
         \ingroup QCustomUi
         \inmodule QCustomUi
*/

struct QCtmWindow::Impl
{
    QCtmTitleBar* title{ nullptr };
    QCtmNavigationBar* navigationMenuBar{ nullptr };
    QStatusBar* statusBar{ nullptr };
    QWidget* content{ nullptr };

#ifdef Q_OS_WIN
    QCtmWinFramelessDelegate* delegate{ nullptr };
#else
    QCtmFramelessDelegate* delegate{ nullptr };
#endif
};

/*!
    \brief      Constructs a dialog which is a child of \a parent.
*/
QCtmWindow::QCtmWindow(QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
    , ui(new Ui::QCtmWindow)
{
    m_impl->title = new QCtmTitleBar(this);
    m_impl->title->installEventFilter(this);
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_impl->content = new QWidget(this);
    m_impl->content->setAutoFillBackground(true);
    ui->titleLayout->addWidget(m_impl->title);
    ui->contentLayout->addWidget(m_impl->content);
    m_impl->content->installEventFilter(this);

#ifdef Q_OS_WIN
    m_impl->delegate = new QCtmWinFramelessDelegate(this, m_impl->title);
#else
    m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
#endif
}

/*!
    \brief      Destroys the widget.
*/
QCtmWindow::~QCtmWindow()
{
    delete ui;
}

/*!
    \brief      Sets the given \a statusBar.
    \sa         statusBar, removeStatusBar
*/
void QCtmWindow::setStatusBar(QStatusBar* statusBar)
{
    removeStatusBar();
    m_impl->statusBar = statusBar;
    ui->statusBarLayout->addWidget(statusBar);
}

/*!
    \brief      Returns the status bar.
    \sa         setStatusBar, removeStatusBar
*/
QStatusBar* QCtmWindow::statusBar() const
{
    return m_impl->statusBar;
}

/*!
    \brief      Remove the status bar.
    \sa         setStatusBar, statusBar
*/
void QCtmWindow::removeStatusBar()
{
    if (m_impl->statusBar)
    {
        delete m_impl->statusBar;
        m_impl->statusBar = nullptr;
    }
}

/*!
    \brief      Sets the given \a menuBar.
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
    \brief      Returns the menu bar.
    \sa         setMenuBar, removeMenuBar
*/
QMenuBar* QCtmWindow::menuBar() const
{
    if (m_impl->title)
        return m_impl->title->menuBar();
    return nullptr;
}

/*!
    \brief      Remove the menu bar.
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
    \brief      Sets the given navigation \a bar.
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
    \brief      Returns the navigation bar.
    \sa         setNavigationBar, removeNavigationBar
*/
QCtmNavigationBar* QCtmWindow::navigationBar() const
{
    return m_impl->navigationMenuBar;
}

/*!
    \brief      Remove the navigation bar.
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
    \brief      Sets the given \a titleBar.
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
    \brief      Returns the title bar.
    \sa         setTitleBar, removeTitleBar()
*/
QCtmTitleBar* QCtmWindow::titleBar() const
{
    return m_impl->title;
}

/*!
    \brief      Remove the title bar.
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
    \brief      Sets the given central \a widget.
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
    \brief      Returns the central widget.
    \sa         setCentralWidget
*/
QWidget* QCtmWindow::centralWidget() const
{
    return m_impl->content;
}

/*!
    \brief      Add the given \a moveBar.
                The move bar provide a function that user can drag the move bar to move the window.
    \sa         removeMoveBar
*/
void QCtmWindow::addMoveBar(QWidget* moveBar)
{
    m_impl->delegate->addMoveBar(moveBar);
}

/*!
    \brief      Remove the given \a moveBar.
    \sa         addMoveBar
*/
void QCtmWindow::removeMoveBar(QWidget* moveBar)
{
    m_impl->delegate->removeMoveBar(moveBar);
}

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
void QCtmWindow::setShadowless(bool flag)
{
    m_impl->delegate->setShadowless(flag);
}

/*!
    \if         !defined(Q_OS_WIN)
    \fn         bool QCtmWindow::shadowless() const
    \brief      Get the window is shadowless.
    \sa         setShadowless
    \else
    \internal
    \endif
*/
bool QCtmWindow::shadowless() const
{
    return m_impl->delegate->shadowless();
}
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
bool QCtmWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
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
