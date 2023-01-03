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

#pragma once

#include <QStyledItemDelegate>

#include <memory>

class QCtmMessageViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QCtmMessageViewDelegate(QObject* parent);
    ~QCtmMessageViewDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setDecoration(const QColor& color);
    const QColor& decoration() const;

    void setCloseButtonIcon(const QPixmap& pixmap);
    const QPixmap& closeButtonIcon() const;

    void setTouchControlStyle(bool flag);
    bool touchControlStyle() const;

protected:
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    void drawTitle(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawDateTime(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawCloseButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawDecoration(QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, const QPixmap& pixmap) const;
    void drawBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    QRect doDateTimeRect(const QStyleOptionViewItem& option, const QModelIndex& model) const;
    QRect doTitleRect(const QStyleOptionViewItem& option, const QModelIndex& model) const;
    QRect doCloseBtnRect(const QStyleOptionViewItem& option) const;
signals:
    void closeButtonClicked(const QModelIndex& index);
    void titleClicked(const QModelIndex& index);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
