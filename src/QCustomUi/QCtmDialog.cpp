#include "QCtmDialog.h"
#include "QCtmTitleBar.h"
#include "Private/QCtmFramelessDelegate_p.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QMoveEvent>
#include <QApplication>

struct QCtmDialog::Impl
{
	QCtmTitleBar* title{ nullptr };
	QWidget* content{ nullptr };
	bool showInCenter{ true };
    QCtmFramelessDelegate* delegate{nullptr};
};

QCtmDialog::QCtmDialog(QWidget *parent)
	: QDialog(parent)
	, m_impl(std::make_shared<Impl>())
{
	m_impl->title = new QCtmTitleBar(this);
    m_impl->title->setObjectName("ctmDialogTitleBar");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(1);
	layout->setSpacing(0);

	layout->addWidget(m_impl->title);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
	auto content = new QWidget(this);
	content->setAutoFillBackground(true);
	setWindowFlags(this->windowFlags() | Qt::Dialog);

	setContent(content);
}

QCtmDialog::~QCtmDialog()
{
}

void QCtmDialog::setContent(QWidget* widget)
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

QWidget* QCtmDialog::content() const
{
	return m_impl->content;
}

void QCtmDialog::setShowInCenter(bool showInCenter)
{
	m_impl->showInCenter = showInCenter;
}

bool QCtmDialog::showInCenter() const
{
	return m_impl->showInCenter;
}

QCtmTitleBar* QCtmDialog::titleBar() const
{
    return m_impl->title;
}

void QCtmDialog::setMoveBars(const QWidgetList& moveBars)
{
    for (auto w : moveBars)
        m_impl->delegate->addMoveBar(w);
}

void QCtmDialog::removeMoveBar(QWidget* moveBar)
{
    m_impl->delegate->removeMoveBar(moveBar);
}

void QCtmDialog::setShadowless(bool flag)
{
	m_impl->delegate->setShadowless(flag);
}

bool QCtmDialog::shadowless() const
{
	return m_impl->delegate->shadowless();
}

void QCtmDialog::showEvent(QShowEvent *e)
{
	auto parent = this->parentWidget();
	if (parent)
	{
		if (showInCenter())
		{
			auto globalPos = parent->topLevelWidget()->pos();
			auto size = parent->size();
			auto center = QRect(globalPos, size).center();
			QPoint desPos{ center.x() - this->width() / 2, center.y() - this->height() / 2 };
			normalizes(desPos);
			move(desPos);
		}
	}
	QDialog::showEvent(e);
}

void QCtmDialog::hideEvent(QHideEvent *event)
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
