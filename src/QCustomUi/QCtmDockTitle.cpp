#include "QCtmDockTitle.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QActionEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QToolButton>

struct QCtmDockTitle::Impl
{
	QString title;
	QTextOption textOption;
	QHBoxLayout* layout{ nullptr };
	QHBoxLayout* fixedButtonsLayout{ nullptr };
	QList<QCtmWidgetItemPtr> items;
};

QCtmDockTitle::QCtmDockTitle(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	Q_ASSERT(parent);
	m_impl->title = parent->windowTitle();
	setFocusProxy(parent);
	parent->installEventFilter(this);
	init();
}

QCtmDockTitle::~QCtmDockTitle()
{
}

void QCtmDockTitle::init()
{
	m_impl->textOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	auto baseLayout = new QHBoxLayout(this);
	baseLayout->setMargin(0);
	m_impl->layout = new QHBoxLayout;
	m_impl->layout->setMargin(0);
	m_impl->layout->setSpacing(0);

	baseLayout->addStretch(1);
	baseLayout->addLayout(m_impl->layout);

	m_impl->fixedButtonsLayout = new QHBoxLayout;
	m_impl->fixedButtonsLayout->setMargin(0);
	m_impl->fixedButtonsLayout->setSpacing(0);
	baseLayout->addLayout(m_impl->fixedButtonsLayout);

	auto floatBtn = new QToolButton(this);
	floatBtn->setIconSize({ 16,16 });
	floatBtn->setObjectName("qctmdocking_float_btn");
	floatBtn->setProperty("float", false);
	m_impl->fixedButtonsLayout->addWidget(floatBtn);

	auto closeBtn = new QToolButton(this);
	closeBtn->setObjectName("qctmdocking_close_btn");
	m_impl->fixedButtonsLayout->addWidget(closeBtn);
	connect(closeBtn, &QToolButton::clicked, this, [this]() {
		if (this->parentWidget())
			this->parentWidget()->close();
		});
}

void QCtmDockTitle::paintEvent(QPaintEvent* event)
{
	QStyleOption option;
	option.initFrom(this);

	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);

	auto textWidth = option.fontMetrics.width(m_impl->title);
	auto textRect = QRect(0, 0, textWidth, this->height());
	if (this->hasFocus())
		p.setPen(QPen(option.palette.color(QPalette::HighlightedText)));
	else
		p.setPen(QPen(option.palette.color(QPalette::Text)));
	p.drawText(textRect, m_impl->title, m_impl->textOption);

	auto decorateWidth = this->width() - 10 * 2 - textRect.width() - m_impl->fixedButtonsLayout->geometry().width();
	auto decorateRect = QRect(10 + textWidth, height() / 3, decorateWidth, height() / 3);
	QBrush brush(Qt::BrushStyle::Dense6Pattern);
	if (this->hasFocus())
		brush.setColor(option.palette.color(QPalette::HighlightedText));
	else
		brush.setColor(option.palette.color(QPalette::Text));
	p.setBrush(brush);
	p.fillRect(decorateRect, brush);
}

void QCtmDockTitle::actionEvent(QActionEvent* event)
{
	if (event->type() == QEvent::ActionAdded)
	{
		auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
		Util::addItem(item, m_impl->items, event->before(), m_impl->layout);
	}
	else
	{
		Util::removeItem(event->action(), m_impl->items, m_impl->layout);
	}
}

bool QCtmDockTitle::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == this->parentWidget())
	{
		if (event->type() == QEvent::WindowTitleChange)
		{
			m_impl->title = this->parentWidget()->windowTitle();
			update();
		}
	}
	return false;
}

QSize QCtmDockTitle::sizeHint() const
{
	auto size = qApp->globalStrut();
	size.setHeight(this->style()->pixelMetric(QStyle::PM_TitleBarHeight));
	return size;
}

QSize QCtmDockTitle::minimumSizeHint() const
{
	return sizeHint();
}
