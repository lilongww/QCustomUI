#include "QCtmMainMenuButton_p.h"
#include "QCtmColor.h"

#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QDebug>

struct QCtmMainMenuButton::Impl
{
	bool pressed{ false };
    bool alternateEnable{ true };
    QColor alternateColor{ Qt::white };
};

QCtmMainMenuButton::QCtmMainMenuButton(QWidget *parent)
	: QCtmToolButton(parent)
	, m_impl(std::make_shared<Impl>())
{
	setContentsMargins(QMargins(0, 0, 0, 4));
	connect(this, &QAbstractButton::pressed, this, [=]() {
		m_impl->pressed = true;
		this->setContentsMargins(QMargins(2, 2, 0, 2));
		update(); });
	connect(this, &QAbstractButton::released, this, [=]() {
		m_impl->pressed = false;
		if (isChecked())
			this->setContentsMargins(QMargins(0, 0, 0, 0));
		else
			this->setContentsMargins(QMargins(0, 0, 0, 4));
		update(); });
    connect(this, &QAbstractButton::toggled, this, [=](bool checked) {
        if (isChecked())
            this->setContentsMargins(QMargins(0, 0, 0, 0));
        else
            this->setContentsMargins(QMargins(0, 0, 0, 4));
        if (!hitButton(mapFromGlobal(QCursor::pos())))
        {
            setAttribute(Qt::WA_UnderMouse, false);
        }
        update(); });
}

QCtmMainMenuButton::~QCtmMainMenuButton()
{
}

void QCtmMainMenuButton::setAlternateColor(const QColor& color)
{
    m_impl->alternateColor = color;
    update();
}

const QColor& QCtmMainMenuButton::alternateColor() const
{
    return m_impl->alternateColor;
}

void QCtmMainMenuButton::setAlternateEnable(bool enable)
{
    m_impl->alternateEnable = enable;
    update();
}

bool QCtmMainMenuButton::alternateEnable() const
{
    return m_impl->alternateEnable;
}

void QCtmMainMenuButton::paintEvent(QPaintEvent*)
{
	QStyleOptionButton opt;
	initStyleOption(&opt);

    QPainter p;
    p.setRenderHint(QPainter::HighQualityAntialiasing);
	p.begin(this);
    if (m_impl->alternateEnable)
    {
        QColor bg(m_impl->alternateColor);
        if (opt.state.testFlag(QStyle::State_Sunken))
        {
            bg.setAlpha(255);
        }
        else if (opt.state.testFlag(QStyle::State_MouseOver))
        {
            bg.setAlpha(191);
        }
        else
        {
            bg.setAlpha(255);
        }
        opt.palette.setColor(QPalette::Background, bg);
    }
	auto contentsRect = style()->subElementRect(QStyle::SE_PushButtonContents, &opt, this);
	if (!contentsRect.isValid())
	{
		contentsRect = QRect(0, 0, width(), height());
	}
    //draw background
    p.fillRect(opt.rect, opt.palette.background());
	opt.rect = contentsRect;
	p.end();
	p.begin(this);
    style()->drawControl(QStyle::CE_PushButtonLabel, &opt, &p, this);
	p.end();
	p.begin(this);
    {//draw icon
        auto st = opt.state.testFlag(QStyle::State_Sunken) ? QIcon::On : QIcon::Off;
        auto mode = opt.state.testFlag(QStyle::State_MouseOver) ? QIcon::Active : QIcon::Normal;
        auto sizes = this->icon().availableSizes(mode, st);

        if (sizes.isEmpty())
        {
            sizes = this->icon().availableSizes(QIcon::Mode::Normal, QIcon::Off);
            st = QIcon::Off;
            mode = QIcon::Normal;
        }

        if (!sizes.isEmpty())
        {
            QPixmap &pixmap = this->icon().pixmap(sizes.front(), mode, st);
            this->style()->drawItemPixmap(&p, contentsRect, Qt::AlignRight | Qt::AlignVCenter, pixmap);
        }
    }
	p.end();
}

void QCtmMainMenuButton::initStyleOption(QStyleOptionButton* opt)
{
	ensurePolished();
	opt->initFrom(this);
	opt->features = QStyleOptionButton::DefaultButton | QStyleOptionButton::AutoDefaultButton;;
	if (m_impl->pressed)
	{
		opt->state |= QStyle::State_Sunken;
		opt->state |= QStyle::State_Raised;
	}
	else if (this->isChecked())
	{
		opt->state |= QStyle::State_On;
	}
	opt->text = this->text();
	int left, top, right, bottom;
	this->getContentsMargins(&left, &top, &right, &bottom);
	opt->rect = QRect(left, top, width() - left - right, height() - top - bottom);
}
