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

#include <QAction>
#include <QWidget>

#include <memory>

class QCtmTabWidget;
class QCtmTabCornerLayout;

class QCUSTOMUI_EXPORT QCtmTabPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    QAction* addAction(const QIcon& icon, const QString& text);
    QAction* addAction(const QString& text);
    QAction* insertAction(int index, const QIcon& icon, const QString& text);
    QAction* insertAction(int index, const QString& text);
    int count() const;
    void insertAction(int index, QAction* action);
    QAction* actionAt(int index) const;
    QWidget* takeCentralWidget() const;
    QWidget* centralWidget() const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
    using QWidget::addAction;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void paintEvent(QPaintEvent* event) override;
    void actionEvent(QActionEvent* event) override;
    void setCentralWidget(QWidget* widget);

    QWidget* cornerWidget() const;
    QCtmTabPage(QCtmTabWidget* parent);
    ~QCtmTabPage();
    friend QCtmTabWidget;
    friend QCtmTabCornerLayout;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
