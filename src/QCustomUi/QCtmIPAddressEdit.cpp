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

#include "QCtmIPAddressEdit.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QTextLayout>
#include <QTimer>
#include <QAction>
#include <QClipboard>
#include <QMenu>
#include <QDebug>

constexpr int SECTION_COUNT = 4;
constexpr int SECTION_CURSOR_COUNT = 4;

QTextLayout::FormatRange createFormatRange(int cursor1, int cursor2, const QPalette& palette)
{
	QTextLayout::FormatRange range;
	int local1 = cursor1 % SECTION_CURSOR_COUNT;
	int local2 = cursor2 % SECTION_CURSOR_COUNT;
	range.start = qMin(local1, local2);
	range.length = qMax(local1, local2) - range.start;
	range.format.setBackground(palette.brush(QPalette::Highlight));
	range.format.setForeground(palette.brush(QPalette::HighlightedText));
	return range;
}

void moveFormatRange(QTextLayout::FormatRange& range, int beforeCursor, int afterCursor)
{
	int before = beforeCursor % SECTION_CURSOR_COUNT;
	int after = afterCursor % SECTION_CURSOR_COUNT;

	int anchor;
	auto start = range.start;
	auto end = range.start + range.length;

	if (end > start && before == start)
		anchor = end;
	else if (end > start && before == end)
		anchor = start;
	else
		anchor = before;
	start = qMin(anchor, after);
	end = qMax(anchor, after);
	range.start = start;
	range.length = end - start;
}

struct QCtmIPAddressEdit::Impl
{
	const int verticalMargin{ 1 };
	const int horizontalMargin{ 2 };
	bool frame{ true };
	bool readOnly{ false };
	int cursorPosition{ 0 };
	int ascent{ 0 };
	int pressedSection{ 0 };
	int pressedCursor{ 0 };
	bool pressed{ false };

	QTextLayout textLayouts[SECTION_COUNT];
	QTimer timer;
	bool cursorSwitch{ false };
	QVector<QVector<QTextLayout::FormatRange>> selections;
	QAction* copy, * paste;
};

QCtmIPAddressEdit::QCtmIPAddressEdit(QWidget* parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit));
	setContextMenuPolicy(Qt::CustomContextMenu);
	m_impl->selections.resize(SECTION_COUNT);
	m_impl->timer.setInterval(qApp->cursorFlashTime() / 2);
	setCursor(Qt::IBeamCursor);
	setFocusPolicy(Qt::StrongFocus);
	connect(&m_impl->timer, &QTimer::timeout, this, [=]() {
		m_impl->cursorSwitch = !m_impl->cursorSwitch;
		update();
		});
	connect(this, &QWidget::customContextMenuRequested, this, &QCtmIPAddressEdit::onCustomContextMenuRequested);
	QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
	initActions();
}

QCtmIPAddressEdit::~QCtmIPAddressEdit()
{
}

/**
 * @brief		Set and show ip, the ip format must be legal.
 */
void QCtmIPAddressEdit::setIPAddress(const QString& ip)
{
	auto tmp = ip.split('.');
	if (tmp.size() == 4)
	{
		for (auto str : tmp)
		{
			if (str.size() > 0 && str.size() <= 3)
			{
				bool ok = false;
				auto num = str.toInt(&ok);
				if (!(num >= 0 && num <= 255 && ok))
					return;
			}
			else
				return;
		}
		for (int i = 0; i < SECTION_COUNT; i++)
		{
			auto&& textLayout = m_impl->textLayouts[i];
			setText(textLayout, tmp[i]);
		}
		update();
	}
}

/**
 * @brief		Get current ip string.
 */
QString QCtmIPAddressEdit::ipAddress() const
{
	QStringList ips;
	for (auto&& textLayout : m_impl->textLayouts)
	{
		ips.append(textLayout.text().isEmpty() ? "0" : textLayout.text());
	}
	return ips.join(".");
}

/**
 * @brief		Set the edit control is read only.
 */
void QCtmIPAddressEdit::setReadOnly(bool ro)
{
	m_impl->readOnly = ro;
}

/**
 * @brief		Get the edit control is read only.
 */
bool QCtmIPAddressEdit::isReadOnly() const
{
	return m_impl->readOnly;
}

int QCtmIPAddressEdit::sectionOfCursorPosition(int position) const
{
	auto section = position / SECTION_CURSOR_COUNT;
	return section > SECTION_COUNT - 1 ? SECTION_COUNT - 1 : section;
}

QTextLayout& QCtmIPAddressEdit::textLayout(int pos) const
{
	auto section = sectionOfCursorPosition(pos);
	return m_impl->textLayouts[section];
}

QRect QCtmIPAddressEdit::rectOfIpSection(int section) const
{
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	int dotWidth = opt.fontMetrics.horizontalAdvance('.');
	auto rect = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
	if (rect.isEmpty())
		rect = QRect(QPoint(0, 0), this->rect().size());

	int ipSectionWidth = opt.fontMetrics.horizontalAdvance(" 000 ");
	return { rect.left() + (dotWidth * section) + section * ipSectionWidth , rect.top(), ipSectionWidth, rect.height() };
}

QRect QCtmIPAddressEdit::boundRect(int section, const QRect& rect) const
{
	const auto& layout = m_impl->textLayouts[section];
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	auto br = layout.boundingRect();
	return { qRound(rect.x() + (rect.width() - br.width()) / 2), qRound(rect.y() + (rect.height() - br.height()) / 2), qRound(br.width()), qRound(br.height()) };
}

bool QCtmIPAddressEdit::setText(QTextLayout& textLayout, const QString& text)
{
	if (!text.isEmpty())
	{
		bool ok{ false };
		auto num = text.toInt(&ok);
		if (num < 0 || num > 255)
			return false;
	}
	textLayout.setText(text);

	QStyleOptionFrame opt;
	initStyleOption(&opt);

	m_impl->ascent = redoTextLayout(textLayout) - opt.fontMetrics.ascent();
	emit editChanged();
	return true;
}

void QCtmIPAddressEdit::paintEvent([[maybe_unused]] QPaintEvent* event)
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
		const auto& rect = rectOfIpSection(i);
		const auto& txtRect = boundRect(i, rect);
		m_impl->textLayouts[i].draw(&p, txtRect.topLeft(), m_impl->selections[i], txtRect);
		if (i != SECTION_COUNT - 1)
			p.drawText(QRect(rect.x() + rect.width(), txtRect.y(), opt.fontMetrics.horizontalAdvance('.'), txtRect.height()), ".");
	}
	if (m_impl->cursorSwitch && hasFocus() && !isReadOnly())
	{
		auto&& txtLayout = this->textLayout(m_impl->cursorPosition);
		auto section = sectionOfCursorPosition(m_impl->cursorPosition);
		section = section > SECTION_COUNT - 1 ? SECTION_COUNT - 1 : section;
		const auto& rect = boundRect(section, rectOfIpSection(section));
		txtLayout.drawCursor(&p, QPoint(rect.left(), rect.y() + m_impl->ascent), m_impl->cursorPosition % 4);
	}
}

void QCtmIPAddressEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
	{
		if (isReadOnly())
			return;
		if (hasSelection())
		{
			deleteSelectedText();
		}
		auto&& layout = textLayout(m_impl->cursorPosition);
		auto&& txt = layout.text();
		if (txt.size() <= m_impl->cursorPosition % SECTION_CURSOR_COUNT)
		{
			if (txt.size() < SECTION_CURSOR_COUNT - 1)
			{
				txt.append(event->text());
				if (setText(layout, txt))
				{
					m_impl->cursorPosition++;
					if (txt.size() == SECTION_CURSOR_COUNT - 1
						&& sectionOfCursorPosition(m_impl->cursorPosition) != SECTION_CURSOR_COUNT - 1)
						m_impl->cursorPosition++;
				}
			}
		}
		else if (txt.size() < SECTION_CURSOR_COUNT - 1)
		{
			txt.insert(m_impl->cursorPosition % SECTION_CURSOR_COUNT, event->text());
			if (setText(layout, txt))
				m_impl->cursorPosition++;
		}
	}
	else if (event->key() == Qt::Key_Backspace)
	{
		if (isReadOnly())
			return;
		if (hasSelection())
		{
			deleteSelectedText();
			update();
			return;
		}
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
		if (!event->modifiers().testFlag(Qt::ShiftModifier) && hasSelection())
		{
			clearSelection();
		}
		else if (event->modifiers().testFlag(Qt::ShiftModifier))
		{
			auto start = m_impl->cursorPosition % SECTION_CURSOR_COUNT;
			if (start == 0)
				return;
			auto section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& selection = m_impl->selections[section];
			if (selection.isEmpty())
			{
				QTextLayout::FormatRange&& range = createFormatRange(m_impl->cursorPosition - 1
					, m_impl->cursorPosition
					, this->palette());
				selection.push_back(range);
			}
			else
			{
				auto&& range = selection.front();
				moveFormatRange(range, m_impl->cursorPosition, m_impl->cursorPosition - 1);
			}
			m_impl->cursorPosition--;
		}
		else if (m_impl->cursorPosition > 0)
		{
			m_impl->cursorPosition--;
			auto section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& txtLayout = m_impl->textLayouts[section];
			auto sectionMaxCursor = txtLayout.text().size() + (section)* SECTION_CURSOR_COUNT;
			if (m_impl->cursorPosition > sectionMaxCursor)
				m_impl->cursorPosition = sectionMaxCursor;
		}
	}
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)
	{
		if (!event->modifiers().testFlag(Qt::ShiftModifier) && hasSelection())
		{
			clearSelection();
		}
		else if (event->modifiers().testFlag(Qt::ShiftModifier))
		{
			auto start = m_impl->cursorPosition % SECTION_CURSOR_COUNT;
			if (start == SECTION_CURSOR_COUNT - 1)
				return;
			auto section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& selection = m_impl->selections[section];
			if (selection.isEmpty())
			{
				auto&& range = createFormatRange(m_impl->cursorPosition, m_impl->cursorPosition + 1, this->palette());
				selection.push_back(range);
			}
			else
			{
				auto&& range = selection.front();
				moveFormatRange(range, m_impl->cursorPosition, m_impl->cursorPosition + 1);
			}
			m_impl->cursorPosition++;
		}
		else if (m_impl->cursorPosition < SECTION_CURSOR_COUNT * SECTION_COUNT)
		{
			auto&& section = sectionOfCursorPosition(m_impl->cursorPosition);
			auto&& txtLayout = m_impl->textLayouts[section];
			if (m_impl->cursorPosition % SECTION_CURSOR_COUNT == txtLayout.text().size())
			{
				if (section < SECTION_COUNT - 1)
					m_impl->cursorPosition = (section + 1) * SECTION_CURSOR_COUNT;
			}
			else
				m_impl->cursorPosition++;
		}
	}
	else if (event->key() == Qt::Key_Delete)
	{
		if (isReadOnly())
			return;
		if (hasSelection())
		{
			deleteSelectedText();
			update();
			return;
		}
		auto&& txtLayout = textLayout(m_impl->cursorPosition);
		if (txtLayout.text().size() > m_impl->cursorPosition % 4)
		{
			auto txt = txtLayout.text().remove(m_impl->cursorPosition % 4, 1);
			setText(txtLayout, txt);
		}
	}
	else if (event->text() == ".")
	{
		auto section = sectionOfCursorPosition(m_impl->cursorPosition);
		auto&& txtLayout = m_impl->textLayouts[section];
		if (!txtLayout.text().isEmpty() && section < SECTION_COUNT - 1 && txtLayout.text().size() == m_impl->cursorPosition % 4)
		{
			m_impl->cursorPosition = (section + 1) * SECTION_CURSOR_COUNT;
		}
	}
	else if (event->key() == Qt::Key_Escape)
	{
		clearSelection();
	}
	update();
}

void QCtmIPAddressEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
	{
		clearSelection();
		auto cursor = xToCursor(event->pos().x());
		auto section = sectionOfCursorPosition(cursor);
		QTextLayout::FormatRange range;
		range.start = 0;
		range.length = m_impl->textLayouts[section].text().size();
		m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + range.length;
		range.format.setBackground(palette().brush(QPalette::Highlight));
		range.format.setForeground(palette().brush(QPalette::HighlightedText));
		m_impl->selections[section].push_back(range);
		update();
	}
}

void QCtmIPAddressEdit::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
	{
		clearSelection();
		m_impl->cursorPosition = xToCursor(event->pos().x());
		m_impl->pressedSection = sectionOfCursorPosition(m_impl->cursorPosition);
		m_impl->pressedCursor = m_impl->cursorPosition;
		m_impl->pressed = true;
		update();
	}
	else if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::ShiftModifier))
	{
		auto section = sectionOfCursorPosition(m_impl->cursorPosition);
		auto pressedCursorPosition = xToCursor(event->pos().x());
		if (section != sectionOfCursorPosition(pressedCursorPosition))
			return;
		auto&& selection = m_impl->selections[section];
		if (selection.isEmpty())
		{
			auto&& range = createFormatRange(m_impl->cursorPosition, pressedCursorPosition, this->palette());
			selection.push_back(range);
		}
		else
		{
			auto&& range = selection.front();
			moveFormatRange(range, m_impl->cursorPosition, pressedCursorPosition);
		}
		m_impl->cursorPosition = pressedCursorPosition;
		update();
	}
}

void QCtmIPAddressEdit::mouseMoveEvent(QMouseEvent* event)
{
	if (event->modifiers().testFlag(Qt::NoModifier) && m_impl->pressed)
	{
		auto movedCursorPosition = xToCursor(event->pos().x());
		if (sectionOfCursorPosition(movedCursorPosition) != m_impl->pressedSection)
			return;
		auto&& selection = m_impl->selections[m_impl->pressedSection];
		if (selection.isEmpty())
		{
			auto&& range = createFormatRange(m_impl->pressedCursor, movedCursorPosition, this->palette());
			selection.push_back(range);
			m_impl->cursorPosition = movedCursorPosition;
		}
		else
		{
			auto&& range = selection.front();
			moveFormatRange(range, m_impl->cursorPosition, movedCursorPosition);
			m_impl->cursorPosition = movedCursorPosition;
		}
		update();
	}
}

void QCtmIPAddressEdit::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
		m_impl->pressed = false;
}

void QCtmIPAddressEdit::focusInEvent([[maybe_unused]] QFocusEvent* event)
{
	m_impl->cursorSwitch = true;
	m_impl->timer.start();
	update();
}

void QCtmIPAddressEdit::focusOutEvent(QFocusEvent* event)
{
	m_impl->cursorSwitch = false;
	m_impl->timer.stop();
	Qt::FocusReason reason = event->reason();
	if (reason != Qt::ActiveWindowFocusReason && reason != Qt::PopupFocusReason)
		clearSelection();
	update();
	emit editFinished();
}

QSize QCtmIPAddressEdit::sizeHint() const
{
	return minimumSizeHint();
}

QSize QCtmIPAddressEdit::minimumSizeHint() const
{
	ensurePolished();
	const auto& fm = fontMetrics();
	const auto& margins = this->contentsMargins();
	int h = fm.height() + qMax(2 * m_impl->verticalMargin, fm.leading()) + margins.top() + margins.bottom();
	int w = fm.horizontalAdvance(" 000 . 000 . 000 . 000 ") + margins.left() + margins.right();
	QStyleOptionFrame opt;
	initStyleOption(&opt);
	return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h).
		expandedTo(QApplication::globalStrut()), this));
}

void QCtmIPAddressEdit::initStyleOption(QStyleOptionFrame* option) const
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

int QCtmIPAddressEdit::xToCursor(int x) const
{
	for (int i = 0; i < SECTION_COUNT; i++)
	{
		auto&& rect = rectOfIpSection(i);
		if (i < SECTION_COUNT - 1)
		{
			rect.setWidth(rect.width() + fontMetrics().horizontalAdvance('.'));
		}
		if (rect.contains(x, rect.y()))
		{
			auto bound = boundRect(i, rect);
			x -= bound.x();
			auto&& textLayout = m_impl->textLayouts[i];
			return i * SECTION_CURSOR_COUNT + textLayout.lineAt(0).xToCursor(x);
		}
		else if (i == SECTION_COUNT - 1)
		{
			if (x > rect.right())
			{
				auto bound = boundRect(i, rect);
				x -= bound.x();
				auto&& textLayout = m_impl->textLayouts[i];
				return i * SECTION_CURSOR_COUNT + textLayout.lineAt(0).xToCursor(x);
			}
		}
	}
	return 0;
}

void QCtmIPAddressEdit::clearSelection()
{
	for (auto&& fr : m_impl->selections)
	{
		fr.clear();
	}
}

void QCtmIPAddressEdit::deleteSelectedText()
{
	for (int section = 0; section != SECTION_COUNT; section++)
	{
		auto fr = m_impl->selections[section];
		if (!fr.isEmpty())
		{
			auto&& range = fr.front();
			auto&& textLayout = m_impl->textLayouts[section];
			setText(textLayout, textLayout.text().remove(range.start, range.length));
			m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + textLayout.text().size();
		}
	}
	clearSelection();
}

bool QCtmIPAddressEdit::hasSelection() const
{
	for (auto fr : m_impl->selections)
	{
		if (!fr.isEmpty())
		{
			return true;
		}
	}
	return false;
}

void QCtmIPAddressEdit::initActions()
{
	m_impl->copy = new QAction(tr("Copy"), this);
	m_impl->copy->setShortcut(QKeySequence::Copy);
	m_impl->copy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(m_impl->copy, &QAction::triggered, this, [=]() {
		for (int section = 0; section < SECTION_COUNT; section++)
		{
			auto selection = m_impl->selections[section];
			if (!selection.isEmpty())
			{
				auto&& range = selection.front();
				auto text = m_impl->textLayouts[section].text().mid(range.start, range.length);
				qApp->clipboard()->setText(text);
				return;
			}
		}
		});
	m_impl->paste = new QAction(tr("Paste"), this);
	m_impl->paste->setShortcut(QKeySequence::Paste);
	m_impl->paste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(m_impl->paste, &QAction::triggered, this, [=]() {
		if (isReadOnly())
			return;
		if (qApp->clipboard()->text().isEmpty())
			return;
		if (qApp->clipboard()->text().split(".").size() == 4)
		{
			setIPAddress(qApp->clipboard()->text());
			return;
		}
		auto&& section = sectionOfCursorPosition(m_impl->cursorPosition);
		auto&& txtLayout = m_impl->textLayouts[section];
		auto&& text = txtLayout.text();
		auto&& selection = m_impl->selections[section];
		auto local = m_impl->cursorPosition % SECTION_CURSOR_COUNT;
		if (selection.isEmpty())
		{
			text.insert(local, qApp->clipboard()->text());
			if (text.size() > SECTION_CURSOR_COUNT - 1)
				return;
			setText(txtLayout, text);
			m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + local + 1;
		}
		else
		{
			auto range = selection.front();
			text.replace(range.start, range.length, qApp->clipboard()->text());
			if (text.size() > SECTION_CURSOR_COUNT - 1)
				return;
			setText(txtLayout, text);
			m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + range.start + range.length;
		}
		clearSelection();
		update();
		});

	addAction(m_impl->copy);
	addAction(m_impl->paste);
}

int QCtmIPAddressEdit::redoTextLayout(QTextLayout& textLayout) const
{
	textLayout.clearLayout();
	textLayout.beginLayout();
	QTextLine l = textLayout.createLine();
	textLayout.endLayout();

	return qRound(l.ascent());
}

void QCtmIPAddressEdit::init()
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

void QCtmIPAddressEdit::onCustomContextMenuRequested(const QPoint& pos)
{
	QMenu menu(this);
	bool selected = false;
	for (const auto& selection : m_impl->selections)
	{
		if (!selection.isEmpty())
		{
			if (selection.front().length != 0)
			{
				selected = true;
				break;
			}
		}
	}
	if (selected)
	{
		menu.addAction(m_impl->copy);
	}
	if (!qApp->clipboard()->text().isEmpty())
	{
		if (!isReadOnly())
			menu.addAction(m_impl->paste);
	}

	if (!menu.isEmpty())
		menu.exec(this->mapToGlobal(pos));
}
