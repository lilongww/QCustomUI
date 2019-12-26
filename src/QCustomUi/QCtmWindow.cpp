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

void QCtmWindow::setStatusBar(QStatusBar* statusBar)
{
	removeStatusBar();
	m_impl->statusBar = statusBar;
    auto sg = statusBar->findChild<QSizeGrip*>();
	ui->statusBarLayout->addWidget(statusBar);
}

QStatusBar* QCtmWindow::statusBar() const
{
	return m_impl->statusBar;
}

void QCtmWindow::removeStatusBar()
{
	if (m_impl->statusBar)
	{
		delete m_impl->statusBar;
		m_impl->statusBar = nullptr;
	}
}

void QCtmWindow::setMenuBar(QMenuBar* menuBar)
{
	if (m_impl->title)
	{
		m_impl->title->setMenuBar(menuBar);
	}
}

QMenuBar* QCtmWindow::menuBar() const
{
	if(m_impl->title)
		return m_impl->title->menuBar();
	return nullptr;
}

void QCtmWindow::removeMenuBar()
{
	if (m_impl->title)
	{
		m_impl->title->removeMenuBar();
	}
}

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

QCtmNavigationBar* QCtmWindow::navigationBar() const
{
	return m_impl->navigationMenuBar;
}

void QCtmWindow::removeNavigationBar()
{
	if (m_impl->navigationMenuBar)
	{
        m_impl->delegate->removeMoveBar(m_impl->navigationMenuBar);
		delete m_impl->navigationMenuBar;
		m_impl->navigationMenuBar = nullptr;
	}
}

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

QCtmTitleBar* QCtmWindow::titleWidget() const
{
	return m_impl->title;
}

void QCtmWindow::removeTitleBar()
{
	if (m_impl->title)
	{
        m_impl->delegate->removeMoveBar(m_impl->title);
		delete m_impl->title;
		m_impl->title = nullptr;
	}
}

void QCtmWindow::setCentralWidget(QWidget *widget)
{
	delete m_impl->content;
	m_impl->content = widget;
    widget->setAutoFillBackground(true);
	ui->contentLayout->addWidget(widget);
}

QWidget* QCtmWindow::centralWidget() const
{
	return m_impl->content;
}

#ifndef Q_OS_WIN
void QCtmWindow::setShadowless(bool flag)
{
	m_impl->delegate->setShadowless(flag);
}

bool QCtmWindow::shadowless() const
{
	return m_impl->delegate->shadowless();
}
#endif

bool QCtmWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_impl->title)
    {
        if (event->type() == QEvent::Close|event->type() == QEvent::Hide)
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
    return false;
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
#endif
}
