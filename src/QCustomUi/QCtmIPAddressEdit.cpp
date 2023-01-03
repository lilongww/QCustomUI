/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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

#include <QAccessible>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QTextLayout>
#include <QTimer>

constexpr int SECTION_COUNT        = 4;
constexpr int SECTION_CURSOR_COUNT = 4;

QTextLayout::FormatRange createFormatRange(int cursor1, int cursor2, const QPalette& palette)
{
    QTextLayout::FormatRange range;
    int local1   = cursor1 % SECTION_CURSOR_COUNT;
    int local2   = cursor2 % SECTION_CURSOR_COUNT;
    range.start  = qMin(local1, local2);
    range.length = qMax(local1, local2) - range.start;
    range.format.setBackground(palette.brush(QPalette::Highlight));
    range.format.setForeground(palette.brush(QPalette::HighlightedText));
    return range;
}

void moveFormatRange(QTextLayout::FormatRange& range, int beforeCursor, int afterCursor)
{
    int before = beforeCursor % SECTION_CURSOR_COUNT;
    int after  = afterCursor % SECTION_CURSOR_COUNT;

    int anchor;
    auto start = range.start;
    auto end   = range.start + range.length;

    if (end > start && before == start)
        anchor = end;
    else if (end > start && before == end)
        anchor = start;
    else
        anchor = before;
    start        = qMin(anchor, after);
    end          = qMax(anchor, after);
    range.start  = start;
    range.length = end - start;
}

struct QCtmIPAddressEdit::Impl
{
    const int verticalMargin { 1 };
    const int horizontalMargin { 2 };
    bool frame { true };
    bool readOnly { false };
    int cursorPosition { 0 };
    int ascent { 0 };
    int pressedSection { 0 };
    int pressedCursor { 0 };
    bool pressed { false };

    QTextLayout textLayouts[SECTION_COUNT];
    QTimer timer;
    bool cursorSwitch { false };
    QVector<QVector<QTextLayout::FormatRange>> selections;
    QAction *copy, *paste;
};

/*!
    \class      QCtmIPAddressEdit
    \brief      IPv4 输入框.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmIPAddressEdit.h

    \b          {截图:}
    \image      QCtmIPAddressEditDetail.png

*/

/*!
    \property   QCtmIPAddressEdit::readOnly
    \brief      输入框是否只读.
*/

/*!
    \fn         void QCtmIPAddressEdit::editChanged()
    \brief      当文本变化时发送该信号.
*/

/*!
    \fn         void QCtmIPAddressEdit::editingFinished()
    \brief      编辑完成时发送该信号.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmIPAddressEdit::QCtmIPAddressEdit(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_impl->selections.resize(SECTION_COUNT);
    m_impl->timer.setInterval(qApp->cursorFlashTime() / 2);
    setFocusPolicy(Qt::StrongFocus);
    connect(&m_impl->timer,
            &QTimer::timeout,
            this,
            [=]()
            {
                m_impl->cursorSwitch = !m_impl->cursorSwitch;
                update();
            });
    connect(this, &QWidget::customContextMenuRequested, this, &QCtmIPAddressEdit::onCustomContextMenuRequested);
    updateTextLayout();
    initActions();
    setReadOnly(m_impl->readOnly);
}

/*!
    \brief      析构函数.
*/
QCtmIPAddressEdit::~QCtmIPAddressEdit() {}

/*!
    \brief      设置 \a ip 地址.
    \sa         ipAddress()
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
                bool ok  = false;
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

/*!
    \brief      返回IP地址.
    \sa         setIPAddress
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

/*!
    \brief      设置是否只读 \a ro.
    \sa         isReadOnly()
*/
void QCtmIPAddressEdit::setReadOnly(bool ro)
{
    m_impl->readOnly = ro;
    setAttribute(Qt::WA_MacShowFocusRect, !ro);
    setAttribute(Qt::WA_InputMethodEnabled, !ro);
    setCursor(ro ? Qt::ArrowCursor : Qt::IBeamCursor);
    QEvent event(QEvent::ReadOnlyChange);
    QCoreApplication::sendEvent(this, &event);
    update();
    QAccessible::State changedState;
    changedState.readOnly = ro;
    QAccessibleStateChangeEvent ev(this, changedState);
    QAccessible::updateAccessibility(&ev);
}

/*!
    \brief      返回是否只读.
    \sa         setReadOnly
*/
bool QCtmIPAddressEdit::isReadOnly() const { return m_impl->readOnly; }

/*!
    \brief      Returns the section of the given \a position.
    \sa         rectOfIpSection, boundRect
*/
int QCtmIPAddressEdit::sectionOfCursorPosition(int position) const
{
    auto section = position / SECTION_CURSOR_COUNT;
    return section > SECTION_COUNT - 1 ? SECTION_COUNT - 1 : section;
}

/*!
    \brief      Returns the text layout of the given \a pos.
*/
QTextLayout& QCtmIPAddressEdit::textLayout(int pos) const
{
    auto section = sectionOfCursorPosition(pos);
    return m_impl->textLayouts[section];
}

/*!
    \brief      Returns the rect of the given \a section.
    \sa         boundRect
*/
QRect QCtmIPAddressEdit::rectOfIpSection(int section) const
{
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    int dotWidth = opt.fontMetrics.horizontalAdvance('.');
    auto rect    = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
    if (rect.isEmpty())
        rect = QRect(QPoint(0, 0), this->rect().size());

    int ipSectionWidth = opt.fontMetrics.horizontalAdvance(" 000 ");
    return { rect.left() + (dotWidth * section) + section * ipSectionWidth, rect.top(), ipSectionWidth, rect.height() };
}

/*!
    \brief      Returns the bound rect of the given \a section and \a rect.
    \sa         rectOfIpSection
*/
QRect QCtmIPAddressEdit::boundRect(int section, const QRect& rect) const
{
    const auto& layout = m_impl->textLayouts[section];
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    auto br = layout.boundingRect();
    return { qRound(rect.x() + (rect.width() - br.width()) / 2),
             qRound(rect.y() + (rect.height() - br.height()) / 2),
             qRound(br.width()),
             qRound(br.height()) };
}

/*!
    \brief      Set the \a text to the \a textLayout.
                Returns true if the text number is between 0 and 255.
    \sa         setIPAddress, ipAddress
*/
bool QCtmIPAddressEdit::setText(QTextLayout& textLayout, const QString& text)
{
    if (!text.isEmpty())
    {
        bool ok { false };
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

/*!
    \reimp
*/
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
        const auto& rect    = rectOfIpSection(i);
        const auto& txtRect = boundRect(i, rect);
        m_impl->textLayouts[i].draw(&p, txtRect.topLeft(), m_impl->selections[i], txtRect);
        if (i != SECTION_COUNT - 1)
            p.drawText(QRect(rect.x() + rect.width(), txtRect.y(), opt.fontMetrics.horizontalAdvance('.'), txtRect.height()), ".");
    }
    if (m_impl->cursorSwitch && hasFocus() && !isReadOnly())
    {
        auto&& txtLayout = this->textLayout(m_impl->cursorPosition);
        auto section     = sectionOfCursorPosition(m_impl->cursorPosition);
        section          = section > SECTION_COUNT - 1 ? SECTION_COUNT - 1 : section;
        const auto& rect = boundRect(section, rectOfIpSection(section));
        txtLayout.drawCursor(&p, QPoint(rect.left(), rect.y() + m_impl->ascent), m_impl->cursorPosition % 4);
    }
}

/*!
    \reimp
*/
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
        auto&& txt    = layout.text();
        if (txt.size() <= m_impl->cursorPosition % SECTION_CURSOR_COUNT)
        {
            if (txt.size() < SECTION_CURSOR_COUNT - 1)
            {
                txt.append(event->text());
                if (setText(layout, txt))
                {
                    m_impl->cursorPosition++;
                    if (txt.size() == SECTION_CURSOR_COUNT - 1 &&
                        sectionOfCursorPosition(m_impl->cursorPosition) != SECTION_CURSOR_COUNT - 1)
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
        auto&& txt    = layout.text();
        if (m_impl->cursorPosition % SECTION_CURSOR_COUNT == 0)
        {
            auto section = sectionOfCursorPosition(m_impl->cursorPosition);
            if (section != 0)
            {
                auto&& layout          = m_impl->textLayouts[section - 1];
                auto txt               = layout.text();
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
            auto section     = sectionOfCursorPosition(m_impl->cursorPosition);
            auto&& selection = m_impl->selections[section];
            if (selection.isEmpty())
            {
                QTextLayout::FormatRange&& range = createFormatRange(m_impl->cursorPosition - 1, m_impl->cursorPosition, this->palette());
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
            auto section          = sectionOfCursorPosition(m_impl->cursorPosition);
            auto&& txtLayout      = m_impl->textLayouts[section];
            auto sectionMaxCursor = txtLayout.text().size() + (section)*SECTION_CURSOR_COUNT;
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
            auto section     = sectionOfCursorPosition(m_impl->cursorPosition);
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
            auto&& section   = sectionOfCursorPosition(m_impl->cursorPosition);
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
        auto section     = sectionOfCursorPosition(m_impl->cursorPosition);
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
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit editingFinished();
    }
    update();
}

/*!
    \reimp
*/
void QCtmIPAddressEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
    {
        clearSelection();
        auto cursor  = xToCursor(event->pos().x());
        auto section = sectionOfCursorPosition(cursor);
        QTextLayout::FormatRange range;
        range.start            = 0;
        range.length           = m_impl->textLayouts[section].text().size();
        m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + range.length;
        range.format.setBackground(palette().brush(QPalette::Highlight));
        range.format.setForeground(palette().brush(QPalette::HighlightedText));
        m_impl->selections[section].push_back(range);
        update();
    }
}

/*!
    \reimp
*/
void QCtmIPAddressEdit::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
    {
        clearSelection();
        m_impl->cursorPosition = xToCursor(event->pos().x());
        m_impl->pressedSection = sectionOfCursorPosition(m_impl->cursorPosition);
        m_impl->pressedCursor  = m_impl->cursorPosition;
        m_impl->pressed        = true;
        update();
    }
    else if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::ShiftModifier))
    {
        auto section               = sectionOfCursorPosition(m_impl->cursorPosition);
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

/*!
    \reimp
*/
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

/*!
    \reimp
*/
void QCtmIPAddressEdit::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::NoModifier))
        m_impl->pressed = false;
}

/*!
    \reimp
*/
void QCtmIPAddressEdit::focusInEvent([[maybe_unused]] QFocusEvent* event)
{
    m_impl->cursorSwitch = true;
    m_impl->timer.start();
    update();
}

/*!
    \reimp
*/
void QCtmIPAddressEdit::focusOutEvent(QFocusEvent* event)
{
    m_impl->cursorSwitch = false;
    m_impl->timer.stop();
    Qt::FocusReason reason = event->reason();
    if (reason != Qt::ActiveWindowFocusReason && reason != Qt::PopupFocusReason)
        clearSelection();
    update();
    emit editingFinished();
}

/*!
    \reimp
*/
void QCtmIPAddressEdit::inputMethodEvent(QInputMethodEvent* e)
{
    if (m_impl->readOnly)
        e->ignore();
    else
        e->accept();
}

/*!
    \reimp
*/
bool QCtmIPAddressEdit::event(QEvent* event)
{
    if (event->type() == QEvent::FontChange)
    {
        updateTextLayout();
    }
    return QWidget::event(event);
}

QVariant QCtmIPAddressEdit::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (query == Qt::ImEnabled)
        return false;
    return QWidget::inputMethodQuery(query);
}

/*!
    \reimp
*/
QSize QCtmIPAddressEdit::sizeHint() const { return minimumSizeHint(); }

/*!
    \reimp
*/
QSize QCtmIPAddressEdit::minimumSizeHint() const
{
    ensurePolished();
    const auto& fm      = fontMetrics();
    const auto& margins = this->contentsMargins();
    int h               = fm.height() + qMax(2 * m_impl->verticalMargin, fm.leading()) + margins.top() + margins.bottom();
    int w               = fm.horizontalAdvance(" 000 . 000 . 000 . 000 ") + margins.left() + margins.right();
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h), this);
}

/*!
    \brief      Initialize the style option.
*/
void QCtmIPAddressEdit::initStyleOption(QStyleOptionFrame* option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->rect         = contentsRect();
    option->lineWidth    = m_impl->frame ? style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this) : 0;
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    if (m_impl->readOnly)
        option->state |= QStyle::State_ReadOnly;
    option->features = QStyleOptionFrame::None;
}

/*!
    \brief      Return the cursor position with given x pixel.
*/
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

/*!
    \brief      Clear the selections.
*/
void QCtmIPAddressEdit::clearSelection()
{
    for (auto&& fr : m_impl->selections)
    {
        fr.clear();
    }
}

/*!
    \brief      Delete the selected texts.
*/
void QCtmIPAddressEdit::deleteSelectedText()
{
    for (int section = 0; section != SECTION_COUNT; section++)
    {
        auto fr = m_impl->selections[section];
        if (!fr.isEmpty())
        {
            auto&& range      = fr.front();
            auto&& textLayout = m_impl->textLayouts[section];
            setText(textLayout, textLayout.text().remove(range.start, range.length));
            m_impl->cursorPosition = section * SECTION_CURSOR_COUNT + textLayout.text().size();
        }
    }
    clearSelection();
}

/*!
    \brief      Returns true if has selection.
    \sa         clearSelection, deleteSelectedText
*/
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

/*!
    \brief      Initialize actions.
    \sa         init
*/
void QCtmIPAddressEdit::initActions()
{
    m_impl->copy = new QAction(tr("Copy"), this);
    m_impl->copy->setShortcut(QKeySequence::Copy);
    m_impl->copy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(m_impl->copy,
            &QAction::triggered,
            this,
            [=]()
            {
                for (int section = 0; section < SECTION_COUNT; section++)
                {
                    auto selection = m_impl->selections[section];
                    if (!selection.isEmpty())
                    {
                        auto&& range = selection.front();
                        auto text    = m_impl->textLayouts[section].text().mid(range.start, range.length);
                        qApp->clipboard()->setText(text);
                        return;
                    }
                }
            });
    m_impl->paste = new QAction(tr("Paste"), this);
    m_impl->paste->setShortcut(QKeySequence::Paste);
    m_impl->paste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(m_impl->paste,
            &QAction::triggered,
            this,
            [=]()
            {
                if (isReadOnly())
                    return;
                if (qApp->clipboard()->text().isEmpty())
                    return;
                if (qApp->clipboard()->text().split(".").size() == 4)
                {
                    setIPAddress(qApp->clipboard()->text());
                    return;
                }
                auto&& section   = sectionOfCursorPosition(m_impl->cursorPosition);
                auto&& txtLayout = m_impl->textLayouts[section];
                auto&& text      = txtLayout.text();
                auto&& selection = m_impl->selections[section];
                auto local       = m_impl->cursorPosition % SECTION_CURSOR_COUNT;
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

/*!
    \brief      redo the \a textLayout.
*/
int QCtmIPAddressEdit::redoTextLayout(QTextLayout& textLayout) const
{
    textLayout.clearLayout();
    textLayout.beginLayout();
    QTextLine l = textLayout.createLine();
    textLayout.endLayout();

    return qRound(l.ascent());
}

/*!
    \brief      Initialization
    \sa
*/
void QCtmIPAddressEdit::updateTextLayout()
{
    QTextOption txtOpt;
    txtOpt.setAlignment(Qt::AlignCenter);
    for (int i = 0; i < SECTION_COUNT; i++)
    {
        auto&& textLayout = m_impl->textLayouts[i];
        textLayout.clearLayout();
        textLayout.setCacheEnabled(true);
        textLayout.setTextOption(txtOpt);
        textLayout.setFont(this->font());
        textLayout.beginLayout();
        textLayout.createLine();
        textLayout.endLayout();
    }
}

/*!
    \brief      Pops the context menu at \a pos
    \sa         initActions
*/
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
