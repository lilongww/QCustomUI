#include "QCtmDialog.h"
#include "QCtmTitleBar.h"
#include "Private/QCtmFramelessDelegate_p.h"
#include "QCtmFramelessDelegate_win.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QMoveEvent>
#include <QApplication>

struct QCtmDialog::Impl
{
	QCtmTitleBar* title{ nullptr };
	QWidget* content{ nullptr };
#ifdef Q_OS_WIN
	QCtmWinFramelessDelegate* delegate{ nullptr };
#else
    QCtmFramelessDelegate* delegate{nullptr};
#endif
};

QCtmDialog::QCtmDialog(QWidget *parent)
	: QDialog(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->title = new QCtmTitleBar(this);
    m_impl->title->setObjectName("ctmDialogTitleBar");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	layout->addWidget(m_impl->title);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

#ifdef Q_OS_WIN
	m_impl->delegate = new QCtmWinFramelessDelegate(this, m_impl->title);
#else
	m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
	setWindowFlag(Qt::Dialog);
#endif
	auto content = new QWidget(this);
	content->setAutoFillBackground(true);
	setCentralWidget(content);
}

QCtmDialog::~QCtmDialog()
{
}

/**
 * @brief       设置客户区，如果客户区窗口已存在，前一个客户区窗口将被销毁
 * @param[in]   widget 客户区窗口
 */
void QCtmDialog::setCentralWidget(QWidget* widget)
{
	delete m_impl->content;
	m_impl->content = widget;
    widget->setAutoFillBackground(true);
	layout()->addWidget(widget);
	QVBoxLayout* l = qobject_cast<QVBoxLayout*>(layout());
	if (l)
	{
		l->setStretch(0, 0);
		l->setStretch(1, 1);
	}
	setWindowTitle(widget->windowTitle());
	m_impl->content->installEventFilter(this);
}

/**
 * @brief       获取客户区窗口地址，如果没有设置客户区窗口，默认存在一个QWidget
 * @Return:     客户区窗口地址
 */
QWidget* QCtmDialog::centralWidget() const
{
	return m_impl->content;
}

/**
 * @brief		获取标题栏地址
 */
QCtmTitleBar* QCtmDialog::titleBar() const
{
    return m_impl->title;
}

/**
 * @brief		设置可拖动的控件
 * @param[in]	moveBars 可拖动的控件
 */
void QCtmDialog::setMoveBars(const QWidgetList& moveBars)
{
    for (auto w : moveBars)
        m_impl->delegate->addMoveBar(w);
}

/**
 * @brief		移除可拖动控件
 * @param[in]
 */
void QCtmDialog::removeMoveBar(QWidget* moveBar)
{
    m_impl->delegate->removeMoveBar(moveBar);
}
#ifndef Q_OS_WIN

/**
 * @brief		设置窗口边框是否有阴影，如果窗口需要容纳OpenGL，则应设置为true
 * @param[in]	flag true:无阴影，flase:有阴影
 */
void QCtmDialog::setShadowless(bool flag)
{
	m_impl->delegate->setShadowless(flag);
}

/**
 * @brief		窗口是否有阴影
 */
bool QCtmDialog::shadowless() const
{
	return m_impl->delegate->shadowless();
}
#endif

void QCtmDialog::hideEvent(QHideEvent *)
{
	auto closeBtn = m_impl->title->findChild<QWidget*>("closeBtn");
	if (closeBtn)
	{
		auto e = new QEvent(QEvent::Type::Leave);
		qApp->sendEvent(closeBtn, e);
	}
}

bool QCtmDialog::eventFilter(QObject* o, QEvent* e)
{
	if (o == m_impl->content && e->type() == QEvent::WindowTitleChange)
	{
		setWindowTitle(m_impl->content->windowTitle());
	}
	return QDialog::eventFilter(o, e);
}

bool QCtmDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
#ifdef Q_OS_WIN
	if (!m_impl->delegate)
		return QDialog::nativeEvent(eventType, message, result);
	if (!m_impl->delegate->nativeEvent(eventType, message, result))
		return QDialog::nativeEvent(eventType, message, result);
	else
		return true;
#else
	return QDialog::nativeEvent(eventType, message, result);
#endif
}

void QCtmDialog::normalizes(QPoint& pos)
{
	auto rect = qApp->desktop()->screenGeometry(pos);
	if (pos.x() < rect.left())
	{
		pos.setX(rect.left());
	}

	if (pos.x() + this->width() > rect.right())
	{
		pos.setX(rect.right() - this->width());
	}

	if (pos.y() < rect.top())
	{
		pos.setX(rect.top());
	}

	if (pos.y() + this->height() > rect.bottom())
	{
		pos.setY(rect.bottom() - this->height());
	}
}
