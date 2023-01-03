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

#include "QCtmAbstractMessageTipModel.h"
#include "qcustomui_global.h"

#include <QColor>

class QCUSTOMUI_EXPORT QCtmMessageTipModel : public QCtmAbstractMessageTipModel
{
    Q_OBJECT
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
    Q_PROPERTY(QColor timeColor READ timeColor WRITE setTimeColor)
public:
    QCtmMessageTipModel(QObject* parent);
    ~QCtmMessageTipModel();

    QVariant data(const QModelIndex& index, int role) const override;
    int columnCount(const QModelIndex& parent /* = QModelIndex() */) const override;
    void setTitleColor(const QColor& color);
    const QColor& titleColor() const;
    void setTimeColor(const QColor& color);
    const QColor& timeColor() const;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
