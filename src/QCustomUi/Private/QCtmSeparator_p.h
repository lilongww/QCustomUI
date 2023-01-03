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

#include <QStyleOption>
#include <QWidget>

class QCtmSeparator : public QWidget
{
    Q_OBJECT

public:
    QCtmSeparator(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QCtmSeparator();

    Qt::Orientation orientation() const;

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent*) override;
    void initStyleOption(QStyleOption* opt) const;

public slots:
    void setOrientation(Qt::Orientation orientation);

private:
    Qt::Orientation m_orientation;
};
