/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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

#include <list>

class QCtmDockWidget;

class QCtmDockAreaWidget : public QWidget
{
    Q_OBJECT

public:
    QCtmDockAreaWidget(QWidget* parent = Q_NULLPTR);
    ~QCtmDockAreaWidget();

    void addDockWidget(QCtmDockWidget* dock);
    void insertDockWidget(int index, QCtmDockWidget* dock);
    void removeDockWidget(QCtmDockWidget* dock);
    void removeDockWidget(int index);
    void clear();
    QCtmDockWidget* dockWidget(int index) const;
    int indexOf(QCtmDockWidget* dock) const;
    bool contains(QCtmDockWidget* dock) const;
    int count() const;
    int currentIndex() const;
    QCtmDockWidget* currentDockWidget() const;
    void merage(QCtmDockAreaWidget* other);
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
