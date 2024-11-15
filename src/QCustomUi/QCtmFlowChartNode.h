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

#include <QGraphicsObject>
#include <QString>

#include <memory>

class QCUSTOMUI_EXPORT QCtmFlowChartNode : public QGraphicsObject
{
public:
    enum class Type
    {
        Start,
        Flow,
        Judgment,
        Manual,
        Line,
        End,
    };
    QCtmFlowChartNode();
    virtual ~QCtmFlowChartNode();
    virtual Type type() const                   = 0;
    virtual bool allowLinkType(Type type) const = 0;
    virtual const std::vector<QCtmFlowChartNode>& nexts() const;
    virtual void setText(const QString& text);
    virtual QString text() const;
signals:
    void textChanged(const QString& text);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

class QCUSTOMUI_EXPORT QCtmFlowChartStartNode : public QCtmFlowChartNode
{
public:
};