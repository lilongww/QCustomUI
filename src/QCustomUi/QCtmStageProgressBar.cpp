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
	std::vector<QString> texts{ static_cast<size_t>(stageCount) };
	QPen stageTextIndexColor{ Qt::white };
	QBrush rateBrush{ QColor{0x3580ce} };
};

QCtmStageProgressBar::QCtmStageProgressBar(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	resize(0, 0);
}

QCtmStageProgressBar::~QCtmStageProgressBar()
{
}

/**
 * @brief		Set the progress bar orientation.
 */
void QCtmStageProgressBar::setOrientation(Qt::Orientation orientation)
{
	if (m_impl->orientation == orientation)
		return;
	m_impl->orientation = orientation;
	updateGeometry();
}

/**
 * @brief		The progress bar orientation.
 */
Qt::Orientation QCtmStageProgressBar::orientation() const
{
	return m_impl->orientation;
}

/**
 * @brief		Set the stage count.
 */
void QCtmStageProgressBar::setStageCount(int count)
{
	if (count < 2)
	{
		qDebug() << "stage count must be greater than or equal to 2";
		return;
	}
	if (count == m_impl->stageCount)
		return;
	m_impl->stageCount = count;
	m_impl->texts.resize(count);
	updateGeometry();
}

/**
 * @brief		Get current stage count;
 */
int QCtmStageProgressBar::stageCount() const
{
	return m_impl->stageCount;
}

/**
 * @brief		Set the stage circle radius pixel count.
 */
void QCtmStageProgressBar::setStageCircleRadius(int radius)
{
	if (m_impl->stageCricleRadius == radius)
		return;
	m_impl->stageCricleRadius = radius;
	updateGeometry();
}

/**
 * @brief		The stage circle radius pixel count.
 */
int QCtmStageProgressBar::stageCricleRadius() const
{
	return m_impl->stageCricleRadius;
}

/**
 * @brief		Set the stage text.
 */
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

/**
 * @brief		The stage text.
 */
QString QCtmStageProgressBar::text(int stage) const
{
	if (stage >= m_impl->stageCount)
	{
		qDebug() << "stage out of range";
		return {};
	}
	return m_impl->texts[stage];
}

/**
 * @brief		Set the text is visible.
 */
void QCtmStageProgressBar::setTextVisible(bool flag)
{
	m_impl->textVisible = flag;
	updateGeometry();
}

/**
 * @brief		Get current stage text is visible.
 */
bool QCtmStageProgressBar::textVisible() const
{
	return m_impl->textVisible;
}

/**
 * @brief		Set the progress bar value.
 */
void QCtmStageProgressBar::setValue(int value)
{
	m_impl->value = value;
	update();
}

/**
 * @brief		Get the progress bar value.
 */
int QCtmStageProgressBar::value() const
{
	return m_impl->value;
}

/**
 * @brief		Set the progress bar maximum value.
 */
void QCtmStageProgressBar::setMaximum(int max)
{
	m_impl->maximum = max;
	update();
}

/**
 * @brief		Get the progress bar maximum value.
 */
int QCtmStageProgressBar::maximum() const
{
	return m_impl->maximum;
}

/**
 * @brief		Set the progress bar minimum value.
 */
void QCtmStageProgressBar::setMinimum(int min)
{
	m_impl->minimum = min;
	update();
}

/**
 * @brief		Get the progress bar minimum value.
 */
int QCtmStageProgressBar::minimum() const
{
	return m_impl->minimum;
}

/**
 * @brief		Set the text in stage circle color.
 */
void QCtmStageProgressBar::setStageIndexTextColor(const QPen& color)
{
	m_impl->stageTextIndexColor = color;
	update();
}

/**
 * @brief		Get the text in stage circle color.
 */
const QPen& QCtmStageProgressBar::stageIndexTextColor() const
{
	return m_impl->stageTextIndexColor;
}

/**
 * @brief		Set the progress bar rate background color.
 */
void QCtmStageProgressBar::setRateBackground(const QBrush& brush)
{
	m_impl->rateBrush = brush;
	update();
}

/**
 * @brief		Get the progress bar rate background color.
 */
const QBrush& QCtmStageProgressBar::rateBackground() const
{
	return m_impl->rateBrush;
}

void QCtmStageProgressBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
	QRect channel;
	QPainterPath path;
	path.setFillRule(Qt::FillRule::WindingFill);

	for (int index = 0; index < m_impl->stageCount; index++)
	{
		const auto& rect = doStageRect(index);
		path.addEllipse(rect);
		if (m_impl->orientation == Qt::Horizontal)
		{
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
		else
		{
			if (index == 0)
			{
				channel.setLeft(rect.center().x() - m_impl->stageCricleRadius / 4);
				channel.setRight(rect.center().x() + m_impl->stageCricleRadius / 4);
				channel.setBottom(rect.center().y());
			}
			else if (index == m_impl->stageCount - 1)
			{
				channel.setTop(rect.center().y());
			}
		}
	}
	path.addRect(channel);

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.fillPath(path, this->palette().window());

	auto&& boundRect = path.boundingRect();
	auto v = static_cast<double>(m_impl->value) / m_impl->maximum;
	if (m_impl->orientation == Qt::Horizontal)
		boundRect.setWidth(boundRect.width() * v);
	else
		boundRect.setTop(boundRect.height() * (1 - v) + boundRect.top());
	p.save();
	p.setClipRect(boundRect);
	p.fillPath(path, m_impl->rateBrush);
	p.restore();
	for (int index = 0; index < m_impl->stageCount; index++)
	{
		const auto& rect = doStageRect(index);
		drawStage(&p, index, rect, m_impl->value);
		drawText(&p, index, doTextRect(index), m_impl->texts[index]);
	}
}

void QCtmStageProgressBar::drawStage([[maybe_unused]] QPainter* p, [[maybe_unused]] int index, [[maybe_unused]] const QRectF& rect, [[maybe_unused]] int value) const
{
	p->save();
	p->setPen(m_impl->stageTextIndexColor);
	p->drawText(rect, QString::number(index + 1), QTextOption(Qt::AlignCenter));
	p->restore();
}

void QCtmStageProgressBar::drawText(QPainter* p, [[maybe_unused]] int index, const QRectF& rect, const QString& text) const
{
	p->save();
	p->setPen(QPen(this->palette().windowText().color()));
	p->drawText(rect, text, QTextOption(m_impl->orientation == Qt::Horizontal ? Qt::AlignCenter : Qt::AlignVCenter | Qt::AlignLeft));
	p->restore();
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
	QSize size = QSize(doMinimumWidth(), doMinimumHeigh());
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

int QCtmStageProgressBar::doMinimumWidth() const
{
	if (Qt::Horizontal == m_impl->orientation)
	{
		auto step = (m_impl->stageCricleRadius * 2 * m_impl->stageCount + m_impl->stageCricleRadius * 3 * (m_impl->stageCount - 1) - m_impl->stageCricleRadius) / m_impl->stageCount;
		if (m_impl->textVisible)
		{
			int w{ 0 };
			auto beginLen{ 0 };
			auto endLen{ 0 };
			for (int i = 0; i < m_impl->texts.size(); i++)
			{
				const auto& text = m_impl->texts[i];
				auto len = fontMetrics().horizontalAdvance(text);
				if (w < len)
					w = len;
				if (i == 0)
					beginLen = len / 2 > m_impl->stageCricleRadius ? len / 2 - m_impl->stageCricleRadius : 0;
				if (i == m_impl->texts.size() - 1)
					endLen = len / 2 > m_impl->stageCricleRadius ? len / 2 - m_impl->stageCricleRadius : 0;
			}
			auto progressPadding = std::max(beginLen, endLen);
			constexpr int space = 10;
			step = step > w + space ? step : (w + space);
			return step * (m_impl->stageCount - 2) + progressPadding * 2 + space;
		}
		else
			return step * m_impl->stageCount;
	}
	else
	{
		if (m_impl->textVisible)
		{
			int len = 0;
			for (const auto& text : m_impl->texts)
				len = qMax(len, fontMetrics().size(0, text).width());
			return len + m_impl->stageCricleRadius * 2 + 10;
		}
		else
		{
			return m_impl->stageCricleRadius * 2;
		}
	}
}

int QCtmStageProgressBar::doMinimumHeigh() const
{
	if (Qt::Horizontal == m_impl->orientation)
		return fontMetrics().height() + (m_impl->textVisible ? fontMetrics().height() + fontMetrics().leading() : 0);
	else
	{
		auto step = (m_impl->stageCricleRadius * 2 * m_impl->stageCount + m_impl->stageCricleRadius * 2 * (m_impl->stageCount - 1) - m_impl->stageCricleRadius) / m_impl->stageCount;

		int endLen = 0;
		int len = 0;
		for (int i = 0; i < m_impl->texts.size(); i++)
		{
			len = qMax(len, fontMetrics().size(0, m_impl->texts[i]).height());
			if (i == 0)
				endLen = len;
		}
		step = step < len + fontMetrics().leading() * 2 ? len + fontMetrics().leading() * 2 : step;
		return step * m_impl->stageCount + (endLen > m_impl->stageCricleRadius ? endLen - m_impl->stageCricleRadius : 0);
	}
}

QRectF QCtmStageProgressBar::doStageRect(int index) const
{
	QStyleOptionProgressBar opt;
	initStyleOption(&opt);

	auto rect = style()->subElementRect(QStyle::SE_ProgressBarContents, &opt, this);

	if (m_impl->orientation == Qt::Horizontal)
	{
		if (!m_impl->textVisible)
		{
			auto step = (rect.width() - m_impl->stageCricleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
			return QRectF(index * step + rect.x(), rect.top() + rect.height() / 2 - m_impl->stageCricleRadius, m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
		}
		else
		{
			auto firstTextLen = fontMetrics().horizontalAdvance(*m_impl->texts.begin());
			auto beginLen = firstTextLen / 2 > m_impl->stageCricleRadius ? firstTextLen / 2 - m_impl->stageCricleRadius : 0;
			auto endTextLen = fontMetrics().horizontalAdvance(*m_impl->texts.rbegin());
			auto endLen = endTextLen / 2 > m_impl->stageCricleRadius ? endTextLen / 2 - m_impl->stageCricleRadius : 0;
			auto progressPadding = std::max(beginLen, endLen);
			auto step = (rect.width() - m_impl->stageCricleRadius * 2 - progressPadding * 2) / static_cast<double>(m_impl->stageCount - 1);
			return QRectF(index * step + rect.x() + progressPadding, rect.top() + rect.height() / 2 - minimumSizeHint().height() / 2, m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
		}
	}
	else
	{
		if (!m_impl->textVisible)
		{
			auto step = (rect.height() - m_impl->stageCricleRadius * 2) / static_cast<double>(m_impl->stageCount - 1);
			return QRectF(rect.x() + rect.width() / 2 - m_impl->stageCricleRadius, index * step + rect.x(), m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
		}
		else
		{
			auto endLen = fontMetrics().size(0, *m_impl->texts.begin()).height();
			auto step = (rect.height() - qMax(endLen, m_impl->stageCricleRadius) - m_impl->stageCricleRadius) / static_cast<double>(m_impl->stageCount - 1);
			return QRectF(rect.x(), rect.top() + step * (m_impl->stageCount - 1 - index), m_impl->stageCricleRadius * 2, m_impl->stageCricleRadius * 2);
		}
	}
}

QRectF QCtmStageProgressBar::doTextRect(int index) const
{
	const auto& size = fontMetrics().size(0, m_impl->texts[index]);
	const auto& stageRect = doStageRect(index);
	if (m_impl->orientation == Qt::Horizontal)
	{
		return QRectF(stageRect.center().x() - size.width() / 2
			, stageRect.bottom() + fontMetrics().leading()
			, size.width()
			, size.height());
	}
	else
	{
		return QRectF(stageRect.right() + 10, stageRect.top(), size.width(), qMax(size.height(), m_impl->stageCricleRadius * 2));
	}
}
