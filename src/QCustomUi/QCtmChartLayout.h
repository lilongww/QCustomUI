/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2022 LiLong                                              **
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

#include "QCtmChartItem.h"

#include <QMargins>

class QCUSTOMUI_EXPORT QCtmChartLayout : public QCtmChartItem
{
    Q_OBJECT
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
public:
    QCtmChartLayout(QCtmAbstractChartView* parent);
    ~QCtmChartLayout();

    void invalidate();
    void setMargins(const QMargins& margin);
    const QMargins& margins() const;

protected:
    void onResized(const QSize& size) override;
    virtual void calcSize(const QSize& size) = 0;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};