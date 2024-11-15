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

#include "QCtmFlowChartNode.h"

struct QCtmFlowChartNode::Impl
{
    std::vector<QCtmFlowChartNode> nexts;
    QString text;
};

QCtmFlowChartNode::QCtmFlowChartNode() : m_impl(std::make_unique<Impl>()) {}

QCtmFlowChartNode::~QCtmFlowChartNode() {}

const std::vector<QCtmFlowChartNode>& QCtmFlowChartNode::nexts() const { return m_impl->nexts; }

void QCtmFlowChartNode::setText(const QString& text)
{
    m_impl->text = text;
    emit textChanged(text);
    update();
}

QString QCtmFlowChartNode::text() const { return m_impl->text; }
