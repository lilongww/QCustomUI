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

#include "QCtmMessageTipData.h"
#include "QCtmMessageViewDelegate_p.h"

#include <QApplication>
#include <QColor>
#include <QLabel>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QToolTip>

Q_CONSTEXPR int margin   = 6;
Q_CONSTEXPR int decorate = 5;
Q_CONSTEXPR int space    = 3;
Q_CONSTEXPR int padding  = 5;

struct QCtmMessageViewDelegate::Impl
{
    bool closeBtnHoverd { false };
    bool closeBtnPressed { false };

    bool titlePressed { false };
    QModelIndex currentIndex;

    QColor decoration;
    QPixmap closeButtonIcon;
    bool touchControlStyle { false };
};

QCtmMessageViewDelegate::QCtmMessageViewDelegate(QObject* parent) : QStyledItemDelegate(parent), m_impl(std::make_unique<Impl>()) {}

QCtmMessageViewDelegate::~QCtmMessageViewDelegate() {}

void QCtmMessageViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto opt = option;
    opt.rect = option.rect - QMargins(margin, margin, margin, margin);
    drawDecoration(painter, opt, QRect(opt.rect.left(), opt.rect.top(), decorate, opt.rect.height()), QPixmap());

    opt.rect = option.rect - QMargins(decorate + margin, margin, margin, margin);
    drawBackground(painter, opt, index);
    drawTitle(painter, option, index);
    drawDateTime(painter, option, index);
    drawCloseButton(painter, option, index);
}

QSize QCtmMessageViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto w = qobject_cast<const QListView*>(option.widget);
    if (!w)
        return QStyledItemDelegate::sizeHint(option, index);

    auto font = option.font;
    font.setBold(true);
    font.setUnderline(true);
    QFontMetrics fm(font);
    auto model        = index.model();
    const auto& title = model->data(model->index(index.row(), QCtmMessageTipData::Title), Qt::DisplayRole).toString();
    const auto& time  = model->data(model->index(index.row(), QCtmMessageTipData::Time), Qt::DisplayRole).toString();
    auto rect         = fm.boundingRect(
        QRect(0, 0, w->viewport()->size().width() - margin * 2 - m_impl->closeButtonIcon.width() - space * 2 - decorate - padding * 2, 0),
        Qt::TextWordWrap | Qt::AlignJustify,
        title + "\n" + time);
    rect.setHeight(rect.height() + margin * 2 + padding * 2);
    return rect.size();
}

void QCtmMessageViewDelegate::setDecoration(const QColor& color) { m_impl->decoration = color; }

const QColor& QCtmMessageViewDelegate::decoration() const { return m_impl->decoration; }

void QCtmMessageViewDelegate::setCloseButtonIcon(const QPixmap& pixmap) { m_impl->closeButtonIcon = pixmap; }

const QPixmap& QCtmMessageViewDelegate::closeButtonIcon() const { return m_impl->closeButtonIcon; }

void QCtmMessageViewDelegate::setTouchControlStyle(bool flag) { m_impl->touchControlStyle = flag; }

bool QCtmMessageViewDelegate::touchControlStyle() const { return m_impl->touchControlStyle; }

void QCtmMessageViewDelegate::drawTitle(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto model        = index.model();
    const auto& title = model->data(model->index(index.row(), QCtmMessageTipData::Title), Qt::DisplayRole).toString();

    QTextOption to;
    to.setWrapMode(QTextOption::WordWrap);
    to.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    auto font = option.font;
    painter->save();
    if (m_impl->touchControlStyle || option.state.testFlag(QStyle::State_MouseOver))
    {
        font.setUnderline(true);
    }
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(model->data(model->index(index.row(), QCtmMessageTipData::Title), Qt::ForegroundRole).value<QColor>());
    painter->drawText(doTitleRect(option, index), title, to);
    painter->restore();
}

void QCtmMessageViewDelegate::drawDateTime(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto model       = index.model();
    const auto& time = model->data(model->index(index.row(), QCtmMessageTipData::Time), Qt::DisplayRole).toString();

    QTextOption to;
    to.setWrapMode(QTextOption::WordWrap);
    to.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    painter->save();
    painter->setPen(model->data(model->index(index.row(), QCtmMessageTipData::Time), Qt::ForegroundRole).value<QColor>());
    painter->drawText(doDateTimeRect(option, index), time, to);
    painter->restore();
}

void QCtmMessageViewDelegate::drawCloseButton(QPainter* painter,
                                              const QStyleOptionViewItem& option,
                                              [[maybe_unused]] const QModelIndex& index) const
{
    if (!m_impl->touchControlStyle && !option.state.testFlag(QStyle::State_MouseOver))
        return;
    const auto& rect = doCloseBtnRect(option);

    painter->save();
    painter->drawPixmap(rect, m_impl->closeButtonIcon);
    painter->restore();
}

void QCtmMessageViewDelegate::drawDecoration(QPainter* painter,
                                             [[maybe_unused]] const QStyleOptionViewItem& option,
                                             const QRect& rect,
                                             [[maybe_unused]] const QPixmap& pixmap) const
{
    painter->fillRect(rect, QBrush(m_impl->decoration));
}

void QCtmMessageViewDelegate::drawBackground(QPainter* painter,
                                             const QStyleOptionViewItem& option,
                                             [[maybe_unused]] const QModelIndex& index) const
{
    QStyle* style = option.widget ? option.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);
}

QRect QCtmMessageViewDelegate::doDateTimeRect(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& titleRect = doTitleRect(option, index);
    const auto& rect      = option.rect - QMargins(margin + space + decorate + padding,
                                              margin + padding,
                                              margin + space + m_impl->closeButtonIcon.width() + padding,
                                              margin + padding);
    return QRect(titleRect.left(), titleRect.bottom(), rect.width(), rect.height() - titleRect.height());
}

QRect QCtmMessageViewDelegate::doTitleRect(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto& title = index.model()->data(index.model()->index(index.row(), QCtmMessageTipData::Title), Qt::DisplayRole).toString();
    auto rect         = option.rect - QMargins(margin + space + decorate + padding,
                                       margin + padding,
                                       margin + space + m_impl->closeButtonIcon.width() + padding,
                                       margin + padding);
    auto font         = option.font;
    font.setBold(true);
    font.setUnderline(true);
    QFontMetrics fm(font);
    rect = fm.boundingRect(rect, Qt::TextWordWrap | Qt::TextJustificationForced, title);
    return rect;
}

QRect QCtmMessageViewDelegate::doCloseBtnRect(const QStyleOptionViewItem& option) const
{
    const auto& rect = option.rect - QMargins(margin, margin, margin, margin);
    return QRect(rect.right() - margin - m_impl->closeButtonIcon.width(),
                 rect.top() + margin,
                 m_impl->closeButtonIcon.width(),
                 m_impl->closeButtonIcon.height());
}

bool QCtmMessageViewDelegate::editorEvent(QEvent* event,
                                          QAbstractItemModel* model,
                                          const QStyleOptionViewItem& option,
                                          const QModelIndex& index)
{
    auto evt = dynamic_cast<QMouseEvent*>(event);
    if (evt)
    {
        switch (evt->type())
        {
        case QEvent::MouseMove:
            {
                if (index != m_impl->currentIndex)
                {
                    m_impl->currentIndex = index;
                }
                if (doCloseBtnRect(option).contains(evt->pos()))
                {
                    m_impl->closeBtnHoverd = true;
                    emit model->dataChanged(index, index);
                }
                else
                    m_impl->closeBtnHoverd = false;
                auto listView = dynamic_cast<const QListView*>(option.widget);
                if (listView)
                {
                    if (doTitleRect(option, index).contains(evt->pos()))
                    {
                        listView->viewport()->setCursor(Qt::PointingHandCursor);
                    }
                    else
                        listView->viewport()->setCursor(Qt::ArrowCursor);
                }

                break;
            }
        case QEvent::MouseButtonPress:
            {
                if (evt->button() != Qt::LeftButton)
                    break;
                if (doCloseBtnRect(option).contains(evt->pos()))
                    m_impl->closeBtnPressed = true;
                else if (doTitleRect(option, index).contains(evt->pos()))
                    m_impl->titlePressed = true;
                break;
            }
        case QEvent::MouseButtonRelease:
            if (evt->button() != Qt::LeftButton)
                break;
            if (m_impl->closeBtnPressed)
            {
                if (doCloseBtnRect(option).contains(evt->pos()))
                {
                    emit closeButtonClicked(index);
                    auto list = dynamic_cast<const QListView*>(option.widget);
                    if (list)
                    {
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
                        QMouseEvent* e = new QMouseEvent(QEvent::MouseMove, evt->pos(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
#else
                        QMouseEvent* e = new QMouseEvent(
                            QEvent::MouseMove, evt->pos(), evt->globalPosition(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
#endif
                        qApp->postEvent(list->viewport(), e);
                    }
                }
            }

            if (m_impl->titlePressed)
            {
                if (doTitleRect(option, index).contains(evt->pos()))
                {
                    emit titleClicked(index);
                }
            }

            m_impl->titlePressed    = false;
            m_impl->closeBtnPressed = false;
            break;
        default:
            break;
        }
    }
    return false;
}
