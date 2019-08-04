#include "QCtmIPEdit.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QTextLayout>
#include <QTimer>

constexpr int SECTION_COUNT = 4;
constexpr int SECTION_CURSOR_COUNT = 4;

struct QCtmIPEdit::Impl
{
	const int verticalMargin{ 1 };
	const int horizontalMargin{ 2 };
	bool frame{ true };
	bool readOnly{ false };
	int cursorPosition{ 0 };
	int ascent{ 0 };

	QTextLayout textLayouts[SECTION_COUNT];
	QTimer timer;
	bool cursorSwitch{ false };
	QVector<QTextLayout::FormatRange> fr;

};

QCtmIPEdit::QCtmIPEdit(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_impl->timer.setInterval(qApp->cursorFlashTime() / 2);
	m_impl->timer.start();
	setCursor(Qt::IBeamCursor);
	setFocusPolicy(Qt::StrongFocus);
	connect(&m_impl->timer, &QTimer::timeout, this, [=]() {
		m_impl->cursorSwitch = !m_impl->cursorSwitch;
		update();
		});
	QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

QCtmIPEdit::~QCtmIPEdit()
{
}

void QCtmIPEdit::setIP(const QString& ip)
{
	auto tmp = ip.split('.');
	if (tmp.size() == 4)
	{
		for (int i = 0; i < SECTION_COUNT; i++)
		{
			auto&& textLayout = m_impl->textLayouts[i];
			setText(textLayout, tmp[i]);
		}
	}
}

QString QCtmIPEdit::ip() const
{
	QStringList ips;
	for (auto&& textLayout : m_impl->textLayouts)
	{
		ips.append(textLayout.text().isEmpty() ? "0" : textLayout.text());
	}
	return ips.join(".");
}

int QCtmIPEdit::sectionOfCursorPosition(int position) const
{
	auto section = position / SECTION_CURSOR_COUNT;
	return section > SECTION_COUNT - 1 ? SECTION_COUNT - 1 : section;
}

QTextLayout& QCtmIPEdit::textLayout(int pos) const
{
	auto section = sectionOfCursorPosition(pos);
	return m_impl->textLayouts[section];
}

QRect QCtmIPEdit::rectOfIpSection(int section)
{
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	int dotWidth = opt.fontMetrics.width('.');
	auto rect = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
	if (rect.isEmpty())
		rect = QRect(QPoint(0, 0), this->rect().size());

	int ipSectionWidth = opt.fontMetrics.width("000");
	return { rect.left() + (dotWidth * section) + section * ipSectionWidth , rect.top(), ipSectionWidth, rect.height() };
}

QRect QCtmIPEdit::boundRect(int section, const QRect& rect)
{
	auto&& layout = m_impl->textLayouts[section];
	auto&& txt = layout.text();
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	auto w = opt.fontMetrics.width(txt);
	return { rect.x() + (rect.width() - w) / 2, rect.y(), w, rect.height() };
}

void QCtmIPEdit::setText(QTextLayout& textLayout, const QString& text)
{
	textLayout.setText(text);

	QStyleOptionFrame opt;
	initStyleOption(&opt);

	m_impl->ascent = redoTextLayout(textLayout) - opt.fontMetrics.ascent();
}

void QCtmIPEdit::paintEvent(QPaintEvent* event)
{
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	auto rect = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
	if (rect.isEmpty())
		rect = QRect(QPoint(0, 0), this->rect().size());
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, &p, this);

	for (int i = 0; i < SECTION_COUNT; i++)
	{
		auto rect = rectOfIpSection(i);
		auto txtRect = boundRect(i, rect);
		m_impl->textLayouts[i].draw(&p, txtRect.topLeft(), m_impl->fr, txtRect);
		if (i != SECTION_COUNT - 1)
			p.drawText(QRect(rect.x() + rect.width(), rect.y(), opt.fontMetrics.width('.'), rect.height()), ".");
	}
	if (m_impl->cursorSwitch && hasFocus())
	{
		auto&& txtLayout = this->textLayout(m_impl->cursorPosition);
		auto section = sectionOfCursorPosition(m_impl->cursorPosition);
		section = section > 3 ? 3 : section;
		auto rect = boundRect(section, rectOfIpSection(section));
		txtLayout.drawCursor(&p, QPoint(rect.left(), m_impl->ascent), m_impl->cursorPosition % 4);
	}
}

void QCtmIPEdit::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
	{
		auto&& layout = textLayout(m_impl->cursorPosition);
		auto&& txt = layout.text();
		if (txt.size() <= m_impl->cursorPosition)
		{
			if (txt.size() < 3)
			{
				txt.append(event->text());
				m_impl->cursorPosition++;
				if (txt.size() == 3 && sectionOfCursorPosition(m_impl->cursorPosition) != 3)
					m_impl->cursorPosition++;
			}
		}
		else if (txt.size() < 3)
		{
			txt.insert(m_impl->cursorPosition, event->text());
			m_impl->cursorPosition++;
		}

		setText(layout, txt);
	}
	else if (event->key() == Qt::Key_Backspace)
	{
		auto&& layout = textLayout(m_impl->cursorPosition);
		auto&& txt = layout.text();
		if (m_impl->cursorPosition % SECTION_CURSOR_COUNT == 0)
		{
			auto section = sectionOfCursorPosition(m_impl->cursorPosition);
			if (section != 0)
			{
				auto&& layout = m_impl->textLayouts[section - 1];
				auto txt = layout.text();
				m_impl->cursorPosition = (section - 1) * SECTION_CURSOR_COUNT + txt.size();
				if (!txt.isEmpty())
				{
					txt.remove(txt.size() - 1, 1);
					m_impl->cursorPosition--;
				}
				setText(layout, txt);
			}
		}
		else if (txt.size())
		{
			txt.remove(m_impl->cursorPosition % SECTION_CURSOR_COUNT - 1, 1);
			m_impl->cursorPosition--;
			setText(layout, txt);
		}
	}
	else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
	{
		if (m_impl->cursorPosition > 0)
		{
			m_impl->cursorPosition--;
			auto section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& txtLayout = m_impl->textLayouts[section];
			auto sectionMaxCursor = txtLayout.text().size() + (section)* SECTION_CURSOR_COUNT;
			if(m_impl->cursorPosition > sectionMaxCursor)
			m_impl->cursorPosition = sectionMaxCursor;
		}
	}
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)
	{
		if (m_impl->cursorPosition < SECTION_CURSOR_COUNT * SECTION_COUNT)
		{
			auto&& section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& txtLayout = m_impl->textLayouts[section];
			if (m_impl->cursorPosition % 4 == txtLayout.text().size())
			{
				if(section < 3)
					m_impl->cursorPosition = m_impl->textLayouts[section + 1].text().size() + (section + 1) * SECTION_CURSOR_COUNT;
			}
			else
				m_impl->cursorPosition++;
		}
	}
	update();
}

QSize QCtmIPEdit::sizeHint() const
{
	return minimumSizeHint();
}

QSize QCtmIPEdit::minimumSizeHint() const
{
	ensurePolished();
	QFontMetrics fm = fontMetrics();
	int left, top, right, bottom;
	this->getContentsMargins(&left, &top, &right, &bottom);
	int h = fm.height() + qMax(2 * m_impl->verticalMargin, fm.leading()) + top + bottom;
	int w = fm.width("000.000.000.000") + left + right;
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h).
		expandedTo(QApplication::globalStrut()), this));
}

void QCtmIPEdit::initStyleOption(QStyleOptionFrame* option) const
{
	if (!option)
		return;

	option->initFrom(this);
	option->rect = contentsRect();
	option->lineWidth = m_impl->frame ? style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this)
		: 0;
	option->midLineWidth = 0;
	option->state |= QStyle::State_Sunken;
	if (m_impl->readOnly)
		option->state |= QStyle::State_ReadOnly;
	option->features = QStyleOptionFrame::None;
}

int QCtmIPEdit::redoTextLayout(QTextLayout& textLayout) const
{
	textLayout.clearLayout();
	textLayout.beginLayout();
	QTextLine l = textLayout.createLine();
	textLayout.endLayout();

	return qRound(l.ascent());
}

void QCtmIPEdit::init()
{
	QTextOption txtOpt;
	txtOpt.setAlignment(Qt::AlignCenter);
	for (int i = 0; i < SECTION_COUNT; i++)
	{
		auto&& textLayout = m_impl->textLayouts[i];
		textLayout.setCacheEnabled(true);
		textLayout.setTextOption(txtOpt);
		textLayout.setFont(this->font());
		textLayout.beginLayout();
		textLayout.createLine();
		textLayout.endLayout();
	}
}
