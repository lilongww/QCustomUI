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

#include "qcustomui_global.h"

#include <QWidget>

class QCUSTOMUI_EXPORT QCtmToolBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    explicit QCtmToolBox(QWidget* parent = nullptr);
    ~QCtmToolBox();

    void addWidget(const QString& title, QWidget* widget);
    void insertWidget(int index, const QString& title, QWidget* widget);
    void removeWidget(QWidget* widget);
    int indexOf(QWidget* widget) const;
    QWidget* widget(int index) const;
    void setStretchFactor(int index, int stretch);
    void setSizes(const QList<int>& sizes);
    QList<int> sizes() const;
    void setDefaultStretch(int index, int stretch);
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void showEvent([[maybe_unused]] QShowEvent* event);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
