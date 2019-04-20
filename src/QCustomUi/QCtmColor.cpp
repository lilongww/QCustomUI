#include "QCtmColor.h"

#include <QMetaObject>
#include <QMetaEnum>
#include <QDebug>

Q_CONSTEXPR int hRange = 14;
Q_CONSTEXPR int hMask = 359;

Q_CONSTEXPR int sRange = 14;
Q_CONSTEXPR int sMask = 255;

Q_CONSTEXPR int vRange = 123;
Q_CONSTEXPR int vMask = 255;

QColor QCtmColor::generalBackgroundColor(int index)
{
    index = index % 20;
    auto colorEnum = QCtmColor::staticMetaObject.enumerator(0);
    return QColor(colorEnum.value(index));
}

QColor QCtmColor::generalForegroundColor(QColor color)
{
    auto func = [](int v, int range, int mask)
    {
        if (v > range)
            return v - range;
        else
            return v + range;
    };
    color.setHsv(func(color.hslHue(), hRange, hMask)
        , func(color.hslSaturation(), sRange, sMask)
        , func(color.value(), vRange, vMask));
    return color;
}
