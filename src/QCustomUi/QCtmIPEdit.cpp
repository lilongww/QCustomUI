#include "QCtmIPEdit.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QTextLayout>
#include <QTimer>

struct QCtmIPEdit::Impl
{
	QStringList ip;
	const int verticalMargin{ 1 };
	const int horizontalMargin{ 2 };
	bool frame{ true };
	bool readOnly{ false };

	QTextLayout layout;
	QTimer timer;
	bool cursorSwitch{ false };
};

QCtmIPEdit::QCtmIPEdit(QWidget *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->ip = QString("0.0.0.0").split('.');
	m_impl->layout.setText("0.0.0.0");
	m_impl->layout.setCacheEnabled(true);
	m_impl->layout.beginLayout();
	QTextLine line = m_impl->layout.createLine();
	m_impl->layout.endLayout();
	m_impl->timer.setInterval(qApp->cursorFlashTime() / 2);
	m_impl->timer.start();
	setCursor(Qt::IBeamCursor);
	setFocusPolicy(Qt::StrongFocus);
	connect(&m_impl->timer, &QTimer::timeout, this, [=]() {
		m_impl->cursorSwitch = !m_impl->cursorSwitch;
		update();
		});
}

QCtmIPEdit::~QCtmIPEdit()
{
}

void QCtmIPEdit::setIP(const QString& ip)
{
	auto tmp = ip.split('.');
	if (tmp.size() == 4)
	{
		m_impl->ip = tmp;
	}
}

QString QCtmIPEdit::ip() const
{
	return m_impl->ip.join('.');
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

	QTextOption txtOpt;
	txtOpt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	p.drawText(rect, m_impl->ip.join('.'), txtOpt);
	if (m_impl->cursorSwitch && hasFocus())
	{
		m_impl->layout.drawCursor(&p, QPointF(20, 0), 2);
	}
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
