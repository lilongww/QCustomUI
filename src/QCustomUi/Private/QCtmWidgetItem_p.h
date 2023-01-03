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

#include <QWidget>

#include <memory>

class QCtmWidgetItem : public QObject
{
    Q_OBJECT
public:
    QCtmWidgetItem(QAction* action, Qt::Orientation orientation, const QSize& iconSize = QSize(16, 16), QWidget* parent = nullptr);
    ~QCtmWidgetItem();

    void release();

    QAction* action() const;
    QWidget* widget() const;
    bool isCustomWidget() const;
signals:
    void iconSizeChanged(const QSize& size);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

typedef std::shared_ptr<QCtmWidgetItem> QCtmWidgetItemPtr;
