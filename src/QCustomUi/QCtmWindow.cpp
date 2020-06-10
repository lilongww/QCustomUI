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

struct QCtmWindow::Impl
{
	QCtmTitleBar *title{ nullptr };
	QCtmNavigationBar* navigationMenuBar{ nullptr };
	QStatusBar* statusBar{ nullptr };
	QWidget* content{ nullptr };

#ifdef Q_OS_WIN
	QCtmWinFramelessDelegate* delegate{ nullptr };
#else
    QCtmFramelessDelegate* delegate{ nullptr };
#endif
};

QCtmWindow::QCtmWindow(QWidget *parent)
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

QCtmWindow::~QCtmWindow()
{
	delete ui;
}

/**
 * @brief  		设置状态栏，如果已存在statusbar，旧的statusbar将被销毁
 * @param[in]  	statusBar 状态栏窗口
 */
void QCtmWindow::setStatusBar(QStatusBar* statusBar)
{
	removeStatusBar();
	m_impl->statusBar = statusBar;
	ui->statusBarLayout->addWidget(statusBar);
}

/**
 * @brief  		获取状态栏地址，如果没有设置statusbar则返回空
 * @Return:   	状态栏地址
 */
QStatusBar* QCtmWindow::statusBar() const
{
	return m_impl->statusBar;
}

/**
 * @brief  		移除状态栏，如果没有设置statusbar则不会产生任何效果。
 */
void QCtmWindow::removeStatusBar()
{
	if (m_impl->statusBar)
	{
		delete m_impl->statusBar;
		m_impl->statusBar = nullptr;
	}
}

/**
 * @brief  		设置传统菜单栏
 * @param[in]  	menuBar 菜单栏地址
 */
void QCtmWindow::setMenuBar(QMenuBar* menuBar)
{
	if (m_impl->title)
	{
		m_impl->title->setMenuBar(menuBar);
	}
}

/**
 * @brief  		获取传统菜单栏地址
 * @Return:   	菜单栏地址
 */
QMenuBar* QCtmWindow::menuBar() const
{
	if(m_impl->title)
		return m_impl->title->menuBar();
	return nullptr;
}

/**
 * @brief  		移除传统菜单栏
 * @note
 */
void QCtmWindow::removeMenuBar()
{
	if (m_impl->title)
	{
		m_impl->title->removeMenuBar();
	}
}

/**
 * @brief  		设置导航菜单栏
 * @param[in]  	menuBar 菜单栏地址
 */
void QCtmWindow::setNavigationBar(QCtmNavigationBar* menuBar)
{
	removeNavigationBar();
	m_impl->navigationMenuBar = menuBar;
	ui->menuBarLayout->addWidget(menuBar);
    if (!m_impl->title)
        m_impl->delegate->addMoveBar(menuBar);
    else if(!m_impl->title->isVisible())
        m_impl->delegate->addMoveBar(menuBar);
}

/**
 * @brief  		获取导航菜单栏地址
 * @Return:   	菜单栏地址
 */
QCtmNavigationBar* QCtmWindow::navigationBar() const
{
	return m_impl->navigationMenuBar;
}

/**
 * @brief  		移除导航菜单栏
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

/**
 * @brief  		设置标题栏
 * @param[in]  	titleBar 标题栏地址
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

/**
 * @brief  		返回标题栏地址
 * @Return:   	标题栏地址
 */
QCtmTitleBar* QCtmWindow::titleWidget() const
{
	return m_impl->title;
}

/**
 * @brief  		移除标题栏
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

/**
 * @brief  		设置中心窗口
 * @param[in]  	widget 中心窗口指针
 */
void QCtmWindow::setCentralWidget(QWidget *widget)
{
	delete m_impl->content;
	m_impl->content = widget;
    widget->setAutoFillBackground(true);
	ui->contentLayout->addWidget(widget);
	setWindowTitle(widget->windowTitle());
}

/**
 * @brief  		获取中心窗口指针
 * @Return:   	中心窗口指针
 */
QWidget* QCtmWindow::centralWidget() const
{
	return m_impl->content;
}

#ifndef Q_OS_WIN

/**
 * @brief		设置窗口边框是否有阴影，如果窗口需要容纳OpenGL，则应设置为true
 * @param[in]	flag true:无阴影，flase:有阴影
 */
void QCtmWindow::setShadowless(bool flag)
{
	m_impl->delegate->setShadowless(flag);
}

/**
 * @brief		窗口是否有阴影
 */
bool QCtmWindow::shadowless() const
{
	return m_impl->delegate->shadowless();
}
#endif

bool QCtmWindow::eventFilter(QObject *watched, QEvent *event)
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
