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

#include <memory>

#include "QCtmToolButton_p.h"

class QStyleOptionToolButton;

class QCtmUserButton : public QCtmToolButton
{
    Q_OBJECT
public:
    QCtmUserButton(QWidget* parent = nullptr);
    ~QCtmUserButton();

protected:
    void paintEvent(QPaintEvent*) override;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QRect doTextRect(const QStyleOptionToolButton& opt) const;
    QRect doIconRect(const QStyleOptionToolButton& opt) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
