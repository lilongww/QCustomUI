#include "QCtmMessageTipHelper_p.h"

#include <QPainter>
#include <QStyleOption>

#include <assert.h>

struct QCtmMessageTipHelper::Impl
{
	int width{ 14 };
	int height{ 16 };
	int margin{ 5 };
	int radius{ 25 };
	
	QColor tipColor;
};

QCtmMessageTipHelper::QCtmMessageTipHelper(QWidget *parent)
	: QObject(parent)
	, m_impl(std::make_unique<Impl>())
{
	parent->installEventFilter(this);
}

QCtmMessageTipHelper::~QCtmMessageTipHelper()
{
}

void QCtmMessageTipHelper::drawMessageTip(int msgCount,QPainter& p, QWidget* w)
{
	if (!msgCount)
		return;
	if (msgCount > 99)
		msgCount = 99;
	auto& pal = w->palette();
	auto rect = tipsRect(QRect(QPoint(0, 0), w->size()));
	QStyleOption opt;
	opt.initFrom(w);
	p.save();
	p.setPen(Qt::NoPen);
	p.setBrush(m_impl->tipColor);
	p.drawRoundRect(rect, m_impl->radius, m_impl->radius);
	p.restore();

	QTextOption to;
	to.setAlignment(Qt::AlignCenter);
	p.save();
	p.setPen(opt.palette.windowText().color());
	p.drawText(rect, QString::number(msgCount), to);
	p.restore();
}

void QCtmMessageTipHelper::setTipColor(const QColor& color)
{
	m_impl->tipColor = color;
}

const QColor& QCtmMessageTipHelper::tipColor() const
{
	return m_impl->tipColor;
}

QRect QCtmMessageTipHelper::tipsRect(const QRect& rect)
{
	auto nrect = rect.normalized();
	return QRect(nrect.right() - m_impl->width - m_impl->margin, nrect.top() + m_impl->margin, m_impl->width, m_impl->height);
}
