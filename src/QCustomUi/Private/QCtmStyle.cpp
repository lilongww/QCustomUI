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

#include "QCtmStyle_p.h"

#include <QIcon>

QPixmap QCtmStyle::standardPixmap(StandardPixmap standardIcon, const QStyleOption* option, const QWidget* widget) const
{
    // switch (standardIcon)
    //{
    // case SP_MessageBoxInformation:
    //     return QPixmap(":/QCustomUi/Resources/messagebox-info.png");
    // case SP_MessageBoxWarning:
    //     return QPixmap(":/QCustomUi/Resources/messagebox-warning.png");
    // case SP_MessageBoxCritical:
    //     return QPixmap(":/QCustomUi/Resources/messagebox-error.png");
    // case SP_MessageBoxQuestion:
    //     return QPixmap(":/QCustomUi/Resources/messagebox-question.png");
    // }
    return QProxyStyle::standardPixmap(standardIcon, option, widget);
}
