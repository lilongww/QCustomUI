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

#include "qcustomui_global.h"

#include <QObject>
#include <QRect>

#include <memory>

class QPainter;
class QCtmAbstractChartView;
class QCUSTOMUI_EXPORT QCtmChartItem : public QObject
{
    Q_OBJECT

public:
    QCtmChartItem(QCtmAbstractChartView* parent);
    ~QCtmChartItem();
    void setGeometry(const QRect& rect);
    const QRect& geometry() const;
    void setMinimumSize(const QSize& size);
    const QSize& minimumSize() const;
    void setMaximumSize(const QSize& size);
    const QSize& maximumSize() const;

    virtual QCtmAbstractChartView* chart() const;
    virtual void draw(QPainter* p) = 0;
    virtual QSize minimumSizeHint() const;

protected:
    virtual void onResized(const QSize& size);
    virtual void onMoved(const QPoint& point);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
