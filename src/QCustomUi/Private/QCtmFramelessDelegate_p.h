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

#include <QWidget>
#include <memory>

class QPlatformSurfaceEvent;
class QCtmFramelessDelegate : public QObject
{
    Q_OBJECT

public:
    QCtmFramelessDelegate(QWidget* parent, const QWidgetList& moveBars);
    QCtmFramelessDelegate(QWidget* parent, QWidget* title);
    ~QCtmFramelessDelegate();

    void addMoveBar(QWidget* widget);
    void removeMoveBar(QWidget* widget);
    void setShadowless(bool flag);
    bool shadowless() const;

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;
    void resizeEvent(QResizeEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);
    void paintShadow(QPainter& p, int shadowWidth);
    void styleChangeEvent(QEvent* e);
    void platformSurfaceEvent(QPlatformSurfaceEvent* e);

private:
    void region(const QPoint& cursorGlobalPoint);
    void updateLayout();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
