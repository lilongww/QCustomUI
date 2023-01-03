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

#include "QCtmToolBoxItemTitle_p.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

struct QCtmToolBoxItemTitle::Impl
{
    QString title;
    QIcon icon;
};

QCtmToolBoxItemTitle::QCtmToolBoxItemTitle(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setAttribute(Qt::WA_StyledBackground);
    setFocusPolicy(Qt::StrongFocus);
}

QCtmToolBoxItemTitle::~QCtmToolBoxItemTitle() {}

void QCtmToolBoxItemTitle::setTitle(const QString& title) { m_impl->title = title; }

const QString& QCtmToolBoxItemTitle::title() const { return m_impl->title; }

void QCtmToolBoxItemTitle::setIcon(const QIcon& icon) { m_impl->icon = icon; }

const QIcon& QCtmToolBoxItemTitle::icon() const { return m_impl->icon; }

void QCtmToolBoxItemTitle::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    QStyleOptionViewItem opt;
    opt.initFrom(this);
    opt.text                = m_impl->title;
    opt.icon                = m_impl->icon;
    opt.font                = font();
    opt.decorationAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    opt.displayAlignment    = Qt::AlignLeft | Qt::AlignVCenter;
    opt.features            = QStyleOptionViewItem::HasDisplay;
    opt.state.setFlag(QStyle::State_HasFocus, false);
    opt.rect = style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
    if (!opt.icon.isNull())
        opt.features |= QStyleOptionViewItem::HasDecoration;
    style()->drawControl(QStyle::CE_ItemViewItem, &opt, &p, this);
}
