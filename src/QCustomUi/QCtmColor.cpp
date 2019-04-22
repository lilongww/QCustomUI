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
