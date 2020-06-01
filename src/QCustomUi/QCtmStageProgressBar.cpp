#include "QCtmStageProgressBar.h"

#include <QPainter>
#include <QStyleOption>
#include <QPainterPath>
#include <QDebug>

struct QCtmStageProgressBar::Impl
{
	int stageCount{ 5 };
	Qt::Orientation orientation{ Qt::Horizontal };
	int minimum{ 0 };
	int maximum{ 99 };
	int value{ 0 };
	bool textVisible{ false };
	int stageCricleRadius{ 10 };
	std::vector<QString> texts{ stageCount };
};

QCtmStageProgressBar::QCtmStageProgressBar(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
}

QCtmStageProgressBar::~QCtmStageProgressBar()
{
}

void QCtmStageProgressBar::setStageCount(int count)
{
	m_impl->stageCount = count;
	updateGeometry();
}

int QCtmStageProgressBar::stageCount() const
{
	return m_impl->stageCount;
}

void QCtmStageProgressBar::setStageCricleRadius(int radius)
{
	m_impl->stageCricleRadius = radius;
	updateGeometry();
}

int QCtmStageProgressBar::stageCricleRadius() const
{
	return m_impl->stageCricleRadius;
}

void QCtmStageProgressBar::setText(int stage, const QString& text)
{
	if (stage >= m_impl->stageCount)
	{
		qDebug() << "stage out of range";
		return;
	}
	m_impl->texts[stage] = text;
	updateGeometry();
}

QString QCtmStageProgressBar::text(int stage) const
{
	if (stage >= m_impl->stageCount)
	{
		qDebug() << "stage out of range";
		return {};
	}
	return m_impl->texts[stage];
}

void QCtmStageProgressBar::setTextVisible(bool flag)
{
	m_impl->textVisible = flag;
	updateGeometry();
}

bool QCtmStageProgressBar::textVisible() const
{
	return m_impl->textVisible;
}

void QCtmStageProgressBar::setValue(int value)
{
	m_impl->value = value;
	update();
}

int QCtmStageProgressBar::value() const
{
	return m_impl->value;
}

void QCtmStageProgressBar::setMaximum(int max)
{
	m_impl->maximum = max;
	update();
}

int QCtmStageProgressBar::maximum() const
{
	return m_impl->maximum;
}

void QCtmStageProgressBar::setMinimum(int min)
{
	m_impl->minimum = min;
	update();
}

int QCtmStageProgressBar::minimum() const
{
	return m_impl->minimum;
}

void QCtmStageProgressBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.drawRect(QRect{ 0,0,width(),height() });
	QRect channel;
	QPainterPath path;
	for (int index = 0; index < m_impl->stageCount; index++)
	{
		const auto& rect = doStageRect(index);
		path.addEllipse(rect);
		if (index == 0)
		{
			channel.setLeft(rect.center().x());
			channel.setTop(rect.center().y() - m_impl->stageCricleRadius / 4);
			channel.setBottom(rect.center().y() + m_impl->stageCricleRadius / 4);
		}
		else if (index == m_impl->stageCount - 1)
		{
			channel.setRight(rect.center().x());
		}
	}
	path.addRect(channel);
	path.setFillRule(Qt::FillRule::WindingFill);
	p.fillPath(path, this->palette().window());

	auto&& boundRect = path.boundingRect();
	auto v = static_cast<double>(m_impl->value) / m_impl->maximum;
	boundRect.setWidth(boundRect.width() * v);
	p.save();
	p.setClipRect(boundRect);
	p.fillPath(path, Qt::red);
	p.restore();
	for (int index = 0; index < m_impl->stageCount; index++)
	{
		const auto& rect = doStageRect(index);
		drawStage(&p, index, rect);
	}
}

void QCtmStageProgressBar::drawStage([[maybe_unused]] QPainter* p, [[maybe_unused]] int index, [[maybe_unused]] const QRectF& rect) const
{

}

QSize QCtmStageProgressBar::sizeHint() const
{
	return minimumSizeHint();
}

QSize QCtmStageProgressBar::minimumSizeHint() const
{
	ensurePolished();
	QFontMetrics fm = fontMetrics();
	QStyleOptionProgressBar opt;
	initStyleOption(&opt);
	QSize size = QSize(doWidth(), fm.height() + 8 + (m_impl->textVisible ? fm.height() * 2 : 0));
	if (opt.orientation == Qt::Vertical)
		size = size.transposed();
	return style()->sizeFromContents(QStyle::CT_ProgressBar, &opt, size, this);
}

void QCtmStageProgressBar::initStyleOption(QStyleOptionProgressBar* opt) const
{
	if (!opt)
		return;
	opt->initFrom(this);

	if (m_impl->orientation == Qt::Horizontal)
		opt->state |= QStyle::State_Horizontal;
	opt->minimum = m_impl->minimum;
	opt->maximum = m_impl->maximum;
	opt->progress = m_impl->value;  
	opt->textAlignment = Qt::AlignCenter;
	opt->textVisible = m_impl->textVisible;
	opt->orientation = m_impl->orientation;
}

int QCtmStageProgressBar::doWidth() const
{
	auto step = (m_impl->stageCricleRadius * 2 * m_impl->stageCount + m_impl->stageCricleRadius * 3 * (m_impl->stageCount - 1) - m_impl->stageCricleRadius) / m_impl->stageCount;
	if (m_impl->textVisible)
	{
		int w{ 0 };
		for (const auto& text : m_impl->texts)
		{
			if (w < fontMetrics().horizontalAdvance(text))
				w = this->fontMetrics().horizontalAdvance(text);
		}
		step = step > w ? step : w;
	}
	return step * m_impl->stageCount;
}

QRectF QCtmStageProgressBar::doStageRect(int index) const
{
	QStyleOptionProgressBar opt;
	initStyleOption(&opt);

	auto rect = style()->subElementRect(QStyle::SE_ProgressBarContents, &opt, this);

	if (!m_impl->textVisible)
	{
		auto step = (rect.width() - m_impl->stageCricleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
		return QRectF(index * step + rect.x(), rect.top() + rect.height() /2 - m_impl->stageCricleRadius, m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
	}
	else
	{
		return QRectF{};
	}
}
