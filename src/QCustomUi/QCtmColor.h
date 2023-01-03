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

#include <QColor>
#include <QObject>

class QCUSTOMUI_EXPORT QCtmColor : QObject
{
    Q_OBJECT
public:
    QCtmColor() = delete;
    enum Colors
    {
        Color0  = 0x2D74EA,
        Color1  = 0x002050,
        Color2  = 0x35888B,
        Color3  = 0x4D4895,
        Color4  = 0xAB742C,
        Color5  = 0x783913,
        Color6  = 0x55815D,
        Color7  = 0x2D41A7,
        Color8  = 0x19422B,
        Color9  = 0x226546,
        Color10 = 0x2AB0C8,
        Color11 = 0x4D4F51,
        Color12 = 0x288A33,
        Color13 = 0x27245C,
        Color14 = 0xCFA183,
        Color15 = 0x827393,
        Color16 = 0xC43F16,
        Color17 = 0x707FC7,
        Color18 = 0xAD5466,
        Color19 = 0x765542
    };
    Q_ENUM(Colors);

    static QColor generateBackgroundColor(int index);
    static QColor generateForegroundColor(QColor color);
};
