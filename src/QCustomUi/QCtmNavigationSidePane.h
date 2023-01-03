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

#include <QScrollArea>

#include <memory>

class QCtmNavigationBar;

class QCUSTOMUI_EXPORT QCtmNavigationSidePane : public QWidget
{
    Q_OBJECT

public:
    enum class DockArea
    {
        Left,
        Top,
        Right,
        Bottom
    };
    explicit QCtmNavigationSidePane(QCtmNavigationBar* parent);
    QCtmNavigationSidePane(DockArea area, QCtmNavigationBar* parent);
    ~QCtmNavigationSidePane();

    void setDockArea(DockArea area);
    DockArea dockArea() const;
    void setWidget(QWidget* widget);
    QWidget* widget() const;
    QScrollArea* viewContainer() const;
    void setTitleBarVisible(bool visible);
    bool titleBarIsVisible() const;
    void setTitle(const QString& text);
    QString title() const;
    void setPopup(bool popup);
    bool popup() const;
signals:
    void paneClosed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    QSize sizeHint() const override;
    bool eventFilter(QObject* o, QEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;

    virtual QPoint smartPosition(DockArea area) const;
    virtual QSize smartSize(DockArea area) const;

    void setNavigationBar(QCtmNavigationBar* bar);
    QCtmNavigationBar* navigationBar() const;

private:
    void bindAction(QAction* action);
    void paintShadow(int shadowWidth);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    friend class QCtmNavigationBar;
};
