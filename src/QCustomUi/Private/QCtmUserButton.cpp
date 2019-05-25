#include "QCtmUserButton_p.h"

#include <QPainter>
#include <QStyleOption>

struct QCtmUserButton::Impl
{
};

QCtmUserButton::QCtmUserButton(QWidget *parent)
	: QCtmToolButton(parent)
	, m_impl(std::make_shared<Impl>())
{
}

QCtmUserButton::~QCtmUserButton()
{
}

void QCtmUserButton::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	QStyleOptionToolButton opt;
	initStyleOption(&opt);
	
	opt.text = "";
	opt.icon = QIcon();
	this->style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
	auto &&rect = this->style()->subElementRect(QStyle::SE_ToolButtonLayoutItem, &opt, this);

	QIcon::State state = opt.state & QStyle::State_On ? QIcon::On : QIcon::Off;
	QIcon::Mode mode;
	if (!(opt.state & QStyle::State_Enabled))
		mode = QIcon::Disabled;
	else if ((opt.state & QStyle::State_MouseOver) && (opt.state & QStyle::State_AutoRaise))
		mode = QIcon::Active;
	else
		mode = QIcon::Normal;
	auto pm = this->icon().pixmap(this->windowHandle(), opt.rect.size().boundedTo(opt.iconSize),
		mode, state);

	this->style()->drawItemPixmap(&p, doIconRect(), Qt::AlignCenter, pm);

	this->style()->drawItemText(&p
		, doTextRect()
		, Qt::AlignCenter, opt.palette, opt.state.testFlag(QStyle::State_Enabled)
		, this->text());
}

QSize QCtmUserButton::sizeHint() const
{
	auto&& rect = doTextRect();
	return QSize(rect.width() + this->iconSize().width() + 2, rect.height());
}

QSize QCtmUserButton::minimumSizeHint() const
{
	return sizeHint();
}

QRect QCtmUserButton::doTextRect() const
{
	QStyleOptionToolButton opt;
	initStyleOption(&opt);
	auto &&rect = opt.fontMetrics.boundingRect(QRect(), Qt::TextDontClip, this->text());
	rect.moveLeft(this->iconSize().width() + 2);
    rect.setHeight(this->height());
	return std::move(rect);
}

QRect QCtmUserButton::doIconRect() const
{
	return std::move(QRect(0, 0, iconSize().width(), height()));
}
