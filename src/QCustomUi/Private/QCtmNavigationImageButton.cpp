#include "QCtmNavigationImageButton_p.h"

#include <QPainter>
#include <QStyleOption>

struct QCtmNavigationImageButton::Impl
{
	bool pressed = false;
};

QCtmNavigationImageButton::QCtmNavigationImageButton(QWidget *parent)
	: QCtmToolButton(parent)
	, m_impl(std::make_unique<Impl>())
{
	this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
	setContentsMargins(QMargins(5, 0, 5, 0));
	connect(this, &QCtmNavigationImageButton::pressed, this, [=]() { m_impl->pressed = true; update(); });
	connect(this, &QCtmNavigationImageButton::released, this, [=]() { m_impl->pressed = false; update(); });
}


QCtmNavigationImageButton::~QCtmNavigationImageButton()
{
}

void QCtmNavigationImageButton::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setRenderHint(QPainter::HighQualityAntialiasing);
	QStyleOptionButton opt;
	opt.initFrom(this);
	opt.features = QStyleOptionButton::DefaultButton | QStyleOptionButton::AutoDefaultButton;;
	if (m_impl->pressed)
	{
		opt.state |= QStyle::State_Sunken;
		opt.state |= QStyle::State_Raised;
	}
	else if (this->isChecked())
	{
		opt.state |= QStyle::State_On;
	}
	opt.text = this->text();
	style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);
	int left, top, right, bottom;
	this->getContentsMargins(&left, &top, &right, &bottom);

	{//draw icon
		auto st = this->isChecked() ? QIcon::On : QIcon::Off;
		const auto& sizes = this->icon().availableSizes(QIcon::Mode::Normal, st);
		if (!sizes.isEmpty())
		{
			const auto& pixmap = this->icon().pixmap(sizes.front(), QIcon::Normal, st);
			this->style()->drawItemPixmap(&p, opt.rect, Qt::AlignCenter, pixmap);
		}
	}
}

QSize QCtmNavigationImageButton::sizeHint() const
{
	QStyleOptionButton opt;
	opt.initFrom(this);
	auto st = this->isChecked() ? QIcon::On : QIcon::Off;
	const auto& sizes = this->icon().availableSizes(QIcon::Mode::Normal, st);
	if (!sizes.isEmpty())
	{
		const auto& size = this->style()->sizeFromContents(QStyle::CT_PushButton, &opt, sizes.front(), this);
		return size;
	}
	return QSize(-1, -1);
}
