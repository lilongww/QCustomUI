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

#include <QAction>
#include <QMap>
#include <QTabWidget>

#include <memory>

class QCtmTabPage;

class QCUSTOMUI_EXPORT QCtmTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    QCtmTabWidget(QWidget* parent = nullptr);
    ~QCtmTabWidget();

public:
    QCtmTabPage* addTab(QWidget* widget, const QString& label);
    QCtmTabPage* addTab(QWidget* widget, const QIcon& icon, const QString& label);
    QCtmTabPage* insertTab(int index, QWidget* widget, const QString& label);
    QCtmTabPage* insertTab(int index, QWidget* widget, const QIcon& icon, const QString& label);
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);
private slots:
    void onCurrentChanged(int index);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
