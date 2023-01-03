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

#include "QCtmStyleSheet.h"

#include <QFile>

/*!
    \class      QCtmStyleSheet
    \brief      默认样式表.
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmStyleSheet.h
*/

/*!
    \brief      返回默认样式表.
    \sa         QApplication::setStyleSheet
*/
QString QCtmStyleSheet::defaultStyleSheet()
{
    QString qss;
    const QStringList qssFiles { { ":/stylesheet/Resources/stylesheet/default-light.css" },
                                 { ":/stylesheet/Resources/stylesheet/sw-light.css" } };
    for (const auto& qssFile : qssFiles)
    {
        QFile file(qssFile);
        if (file.open(QFile::ReadOnly))
        {
            qss.append(file.readAll());
        }
    }
    return qss;
}
