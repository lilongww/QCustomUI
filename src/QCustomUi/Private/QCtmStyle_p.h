#pragma once

#include <QProxyStyle>

class QCtmStyle : public QProxyStyle
{
public:
	QCtmStyle(QStyle *style = Q_NULLPTR) :QProxyStyle(style){}
    QPixmap standardPixmap(StandardPixmap standardIcon
        , const QStyleOption *option = Q_NULLPTR
        , const QWidget *widget = Q_NULLPTR) const override;
};

