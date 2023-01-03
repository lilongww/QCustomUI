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

#include <QAbstractItemView>

class QCtmClassifyTreeNode;
class QCtmClassifyTreeGroup;
class QCtmClassifyTreeIcon;

class QCUSTOMUI_EXPORT QCtmClassifyTreeView : public QAbstractItemView
{
    Q_OBJECT

public:
    enum Role
    {
        NodeTypeRole = Qt::UserRole + 1
    };
    QCtmClassifyTreeView(QWidget* parent);
    ~QCtmClassifyTreeView();

    void setHorizontalSpace(int space);
    int horizontalSpace() const;
    void setVerticalSpace(int space);
    int verticalSpace() const;
    void setIconItemSize(const QSize& size);
    QSize iconItemSize() const;
    void setIndentation(int i);
    int indentation() const;

    void setModel(QAbstractItemModel* model) override;
    QModelIndex indexAt(const QPoint& point) const override;
    void scrollTo(const QModelIndex& index, QAbstractItemView::ScrollHint hint = EnsureVisible) override;
    QRect visualRect(const QModelIndex& index) const override;

public slots:
    void expandAll();
    void expand(const QModelIndex& index);
    void collapseAll();
    void collapse(const QModelIndex& index);

protected:
    int horizontalOffset() const override;
    int verticalOffset() const override;
    bool isIndexHidden(const QModelIndex& index) const override;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags flags) override;
    QRegion visualRegionForSelection(const QItemSelection& selection) const override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void updateGeometries() override;

    virtual void relayoutNodes();
    virtual void initStyleOption(QCtmClassifyTreeNode* node, QStyleOptionViewItem& option);

private:
    void createNodes();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
