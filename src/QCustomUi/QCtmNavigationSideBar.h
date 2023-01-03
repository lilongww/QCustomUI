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

#include <QWidget>
#include <QWidgetAction>

#include <memory>

class QCUSTOMUI_EXPORT QCtmNavigationSideBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    enum ActionPosition
    {
        Top,
        Bottom
    };
    QCtmNavigationSideBar(QWidget* parent);
    ~QCtmNavigationSideBar();

    QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);
    void addAction(QAction* action, ActionPosition pos);
    QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);
    void insertAction(int index, QAction* action, ActionPosition pos);
    int indexOf(QAction* action) const;
    int count(ActionPosition pos) const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
    QAction* actionAt(int index, ActionPosition pos) const;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void actionEvent(QActionEvent* event) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
