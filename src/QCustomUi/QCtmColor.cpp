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

#include "QCtmColor.h"

#include <QDebug>
#include <QMetaEnum>
#include <QMetaObject>

bool isLight(QColor color) { return 0.213 * color.red() + 0.715 * color.green() + 0.072 * color.blue() > 255 / 2; }

/*!
    \class      QCtmColor
    \brief      部分常用颜色和颜色计算方法.
    \inherits   QObject
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmColor.h
*/

/*!
    \raw HTML
    <style type="text/css" id="colorstyles">
    #color-0 { background-color: #2D74EA; color: #2D74EA }
    #color-1 { background-color: #002050; color: #002050 }
    #color-2 { background-color: #35888B; color: #35888B }
    #color-3 { background-color: #4D4895; color: #4D4895 }
    #color-4 { background-color: #AB742C; color: #AB742C }
    #color-5 { background-color: #783913; color: #783913 }
    #color-6 { background-color: #55815D; color: #55815D }
    #color-7 { background-color: #2D41A7; color: #2D41A7 }
    #color-8 { background-color: #19422B; color: #19422B }
    #color-9 { background-color: #226546; color: #226546 }
    #color-10 { background-color: #2AB0C8; color: #2AB0C8 }
    #color-11 { background-color: #4D4F51; color: #4D4F51 }
    #color-12 { background-color: #288A33; color: #288A33 }
    #color-13 { background-color: #27245C; color: #27245C }
    #color-14 { background-color: #CFA183; color: #CFA183 }
    #color-15 { background-color: #827393; color: #827393 }
    #color-16 { background-color: #C43F16; color: #C43F16 }
    #color-17 { background-color: #707FC7; color: #707FC7 }
    #color-18 { background-color: #AD5466; color: #AD5466 }
    #color-19 { background-color: #765542; color: #765542 }
    </style>
    \endraw

    \enum       QCtmColor::Colors

    \value      Color0
                \tt {\span {id="color-0"} {-------------}}
    \value      Color1
                \tt {\span {id="color-1"} {-------------}}
    \value      Color2
                \tt {\span {id="color-2"} {-------------}}
    \value      Color3
                \tt {\span {id="color-3"} {-------------}}
    \value      Color4
                \tt {\span {id="color-4"} {-------------}}
    \value      Color5
                \tt {\span {id="color-5"} {-------------}}
    \value      Color6
                \tt {\span {id="color-6"} {-------------}}
    \value      Color7
                \tt {\span {id="color-7"} {-------------}}
    \value      Color8
                \tt {\span {id="color-8"} {-------------}}
    \value      Color9
                \tt {\span {id="color-9"} {-------------}}
    \value      Color10
                \tt {\span {id="color-10"} {-------------}}
    \value      Color11
                \tt {\span {id="color-11"} {-------------}}
    \value      Color12
                \tt {\span {id="color-12"} {-------------}}
    \value      Color13
                \tt {\span {id="color-13"} {-------------}}
    \value      Color14
                \tt {\span {id="color-14"} {-------------}}
    \value      Color15
                \tt {\span {id="color-15"} {-------------}}
    \value      Color16
                \tt {\span {id="color-16"} {-------------}}
    \value      Color17
                \tt {\span {id="color-17"} {-------------}}
    \value      Color18
                \tt {\span {id="color-18"} {-------------}}
    \value      Color19
                \tt {\span {id="color-19"} {-------------}}
*/

/*!
    \brief      生成背景色 \a index, 如果 \a index 超过20，从头开始循环.
    \sa         generateForegroundColor
*/
QColor QCtmColor::generateBackgroundColor(int index)
{
    index          = index % 20;
    auto colorEnum = QCtmColor::staticMetaObject.enumerator(0);
    return QColor(colorEnum.value(index));
}

/*!
    \brief      根据 \a color 生成前景色.
    \sa         generateBackgroundColor
*/
QColor QCtmColor::generateForegroundColor(QColor color)
{
    int h, s, l;
    color.toHsl().getHsl(&h, &s, &l);
    QColor target;
    target.setHsl(h, s, isLight(color) ? 45 : 210);
    return target;
}
