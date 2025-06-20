/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
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

#include "QCtmTableItemDelegate.h"

#include <QMouseEvent>

class QCtmTableView;
class QCUSTOMUI_EXPORT QCtmTableViewButtonsDelegate : public QCtmTableItemDelegate
{
    Q_OBJECT
    Q_PROPERTY(bool uniformButtonSize READ uniformButtonSize WRITE setUniformButtonSize)
    Q_PROPERTY(int space READ space WRITE setSpace)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
public:
    explicit QCtmTableViewButtonsDelegate(QCtmTableView* parent);
    ~QCtmTableViewButtonsDelegate();
    void addButton(const QString& text,
                   const QBrush& background,
                   const QBrush& hover,
                   const QBrush& pressed   = {},
                   const QColor& textColor = {});
    void setUniformButtonSize(bool on);
    bool uniformButtonSize() const;
    void setButtonSize(int button, const QSize& size);
    const QSize& buttonSize(int button) const;
    void setSpace(int space);
    int space() const;
    void setAlignment(Qt::Alignment alignment);
    Qt::Alignment alignment() const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
signals:
    void buttonClicked(int button, const QModelIndex& index);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
    virtual void drawButtons(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void mouseMoveEvent(int button, const QModelIndex& index, QMouseEvent* e);
    virtual void mousePressEvent(int button, const QModelIndex& index, QMouseEvent* e);
    virtual void mouseReleaseEvent(int button, const QModelIndex& index, QMouseEvent* e);
    void update();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
