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

#include <QTextLayout>
#include <QWidget>

#include <memory>

class QStyleOptionFrame;

class QCUSTOMUI_EXPORT QCtmIPAddressEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIPAddress)
public:
    QCtmIPAddressEdit(QWidget* parent);
    ~QCtmIPAddressEdit();

    void setIPAddress(const QString& ip);
    QString ipAddress() const;

    void setReadOnly(bool ro);
    bool isReadOnly() const;
signals:
    void editChanged();
    void editingFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void inputMethodEvent(QInputMethodEvent* e) override;
    bool event(QEvent* event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    int sectionOfCursorPosition(int position) const;
    QTextLayout& textLayout(int pos) const;
    QRect rectOfIpSection(int section) const;
    QRect boundRect(int section, const QRect& rect) const;
    bool setText(QTextLayout& textLayout, const QString& text);
    int redoTextLayout(QTextLayout& textLayout) const;
    void initStyleOption(QStyleOptionFrame* option) const;
    int xToCursor(int x) const;
    void clearSelection();
    void deleteSelectedText();
    bool hasSelection() const;
    void initActions();
private slots:
    void updateTextLayout();
    void onCustomContextMenuRequested(const QPoint& pos);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
