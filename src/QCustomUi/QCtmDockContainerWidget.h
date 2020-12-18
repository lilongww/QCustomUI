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

#include "qcustomui_global.h"

#include <QWidget>

class QCtmDockAreaWidget;
class QCtmDockContainerWidget : public QWidget
{
    Q_OBJECT

public:
    enum DockArea
    {
        None,
        Left = 0x01,
        Top = 0x02,
        Right = 0x04,
        Bottom = 0x08,
        Center = 0x10
    };
    Q_DECLARE_FLAGS(DockAreas, DockArea)
public:
    QCtmDockContainerWidget(QWidget* parent);
    ~QCtmDockContainerWidget();

    void addDockAreaWidget(DockArea area, QCtmDockAreaWidget* areaWidget, QCtmDockAreaWidget* targetAreaWidget);
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
