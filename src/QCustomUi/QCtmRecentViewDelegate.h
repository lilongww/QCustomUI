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

#include "qcustomui_global.h"

#include <QStyledItemDelegate>

class QCtmRecentView;
class QCUSTOMUI_EXPORT QCtmRecentViewDelegate : public QStyledItemDelegate
{
    Q_PROPERTY(QIcon topButtonIcon READ topButtonIcon WRITE setTopButtonIcon)
    Q_PROPERTY(bool topButtonVisible READ topButtonVisible WRITE setTopButtonVisible)
    Q_OBJECT
public:
    explicit QCtmRecentViewDelegate(QCtmRecentView* parent);
    ~QCtmRecentViewDelegate();
    void setTopButtonIcon(const QIcon& icon);
    QIcon topButtonIcon() const;
    void setTopButtonVisible(bool v);
    bool topButtonVisible() const;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual void drawIcon(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void drawName(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void drawPath(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void drawTime(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void drawTopButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual QRect doIconRect(const QStyleOptionViewItem& option) const;
    virtual QRect doNameRect(const QStyleOptionViewItem& option) const;
    virtual QRect doPathRect(const QStyleOptionViewItem& option) const;
    virtual QRect doTimeRect(const QStyleOptionViewItem& option) const;
    virtual QRect doTopButtonRect(const QStyleOptionViewItem& option) const;

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
signals:
    void topButtonClicked(const QModelIndex& index);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};