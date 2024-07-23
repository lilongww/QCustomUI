/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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

#include <QLineEdit>
#include <QValidator>

class QCUSTOMUI_EXPORT QCtmHexValidator : public QValidator
{
    Q_OBJECT
public:
    using QValidator::QValidator;
    State validate(QString&, int&) const override;
    void fixup(QString&) const override;
};

class QCUSTOMUI_EXPORT QCtmHexLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QCtmHexLineEdit(QWidget* parent = nullptr);
    ~QCtmHexLineEdit();
};
