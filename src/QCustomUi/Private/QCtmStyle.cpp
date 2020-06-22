#include "QCtmStyle_p.h"

#include <QIcon>

QPixmap QCtmStyle::standardPixmap(StandardPixmap standardIcon
    , const QStyleOption* option
    , const QWidget* widget) const
{
    //switch (standardIcon)
    //{
    //case SP_MessageBoxInformation:
    //    return QPixmap(":/QCustomUi/Resources/messagebox-info.png");
    //case SP_MessageBoxWarning:
    //    return QPixmap(":/QCustomUi/Resources/messagebox-warning.png");
    //case SP_MessageBoxCritical:
    //    return QPixmap(":/QCustomUi/Resources/messagebox-error.png");
    //case SP_MessageBoxQuestion:
    //    return QPixmap(":/QCustomUi/Resources/messagebox-question.png");
    //}
    return QProxyStyle::standardPixmap(standardIcon, option, widget);
}
