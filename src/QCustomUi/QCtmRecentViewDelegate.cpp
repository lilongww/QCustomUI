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
#include "QCtmRecentViewDelegate.h"
#include "QCtmRecentModel.h"

#include <QFontMetricsF>
#include <QPainter>

constexpr static int SpacePixel = 15;

QCtmRecentViewDelegate::QCtmRecentViewDelegate(QObject* parent /*= nullptr*/) : QStyledItemDelegate(parent) {}

QCtmRecentViewDelegate::~QCtmRecentViewDelegate() {}

void QCtmRecentViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.parent().isValid())
        QStyledItemDelegate::paint(painter, option, index);
    else
    {
        auto opt = option;
        opt.text.clear();
        QStyledItemDelegate::paint(painter, opt, index);
        drawIcon(painter, option, index);
        drawName(painter, option, index);
    }
}

QSize QCtmRecentViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto size = QStyledItemDelegate::sizeHint(option, index);
    if (!index.parent().isValid())
        return size;
    else
    {
        auto height = SpacePixel * 3 + option.fontMetrics.height() * 2;
        return size.expandedTo(QSize(size.width(), height));
    }
}

void QCtmRecentViewDelegate::drawIcon(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto icon = index.data(QCtmRecentModel::Roles::Icon).value<QIcon>();
    QIcon::Mode mode;
    if (option.widget->isEnabled())
    {
        mode = option.state.testFlag(QStyle::State_Selected) ? QIcon::Mode::Selected : QIcon::Mode::Normal;
    }
    else
        mode = QIcon::Mode::Disabled;
    painter->drawPixmap(doIconRect(option), icon.pixmap(0, mode, QIcon::State::Off));
}

void QCtmRecentViewDelegate::drawName(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->drawText(
        doNameRect(option), index.data(QCtmRecentModel::Roles::Name).toString(), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
}

void QCtmRecentViewDelegate::drawPath(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->drawText(
        doPathRect(option), index.data(QCtmRecentModel::Roles::Path).toString(), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
}

QRect QCtmRecentViewDelegate::doIconRect(const QStyleOptionViewItem& option) const
{
    const auto& r = option.rect;
    return QRect(r.left() + SpacePixel, r.top() + SpacePixel, 24, 24);
}

QRect QCtmRecentViewDelegate::doNameRect(const QStyleOptionViewItem& option) const
{
    const auto& r = option.rect;
    auto iconRect = doIconRect(option);
    return QRect(iconRect.right() + SpacePixel, iconRect.top(), r.right() - iconRect.right() - SpacePixel, option.fontMetrics.height());
}

QRect QCtmRecentViewDelegate::doPathRect(const QStyleOptionViewItem& option) const
{
    auto nameRect = doNameRect(option);
    return QRect(nameRect.left(), nameRect.bottom() + SpacePixel, nameRect.width(), option.fontMetrics.height());
}
