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

#include "QCtmColor.h"

#include <QMetaObject>
#include <QMetaEnum>
#include <QDebug>

bool isLight(QColor color)
{
	return 0.213 * color.red() + 0.715 * color.green() + 0.072 * color.blue() > 255 / 2;
}

QColor QCtmColor::generalBackgroundColor(int index)
{
	index = index % 20;
	auto colorEnum = QCtmColor::staticMetaObject.enumerator(0);
	return QColor(colorEnum.value(index));
}

QColor QCtmColor::generalForegroundColor(QColor color)
{
	int h, s, l;
	color.toHsl().getHsl(&h, &s, &l);
	QColor target;
	target.setHsl(h, s, isLight(color) ? 45 : 210);
	return target;
}
