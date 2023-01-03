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
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QMouseEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

class QCtmDrawerItemWidget;
class QCUSTOMUI_EXPORT QCtmDrawerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool exclusive READ exclusive WRITE setExclusive)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    explicit QCtmDrawerWidget(QWidget* parent = nullptr);
    ~QCtmDrawerWidget();

    QCtmDrawerItemWidget* addWidget(const QString& title, QWidget* widget);
    QCtmDrawerItemWidget* insertWidget(int index, const QString& title, QWidget* widget);
    void removeItem(QCtmDrawerItemWidget* item);
    int indexOf(QCtmDrawerItemWidget* item) const;
    QCtmDrawerItemWidget* item(int index) const;
    int count() const;
    void setExclusive(bool exclusive);
    bool exclusive() const;
    void setSizes(const QList<int>& sizes);
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void itemExpandChanged(QCtmDrawerItemWidget* item, bool expand);
    void itemTitleClicked(QCtmDrawerItemWidget* item, bool expand);
    void iconSizeChanged(const QSize& size);
public slots:
    void expandAll();
    void collapseAll();

protected:
    bool isAllCollapsed() const;
    int totalSize(const QList<int>& sizes) const;
    void showEvent(QShowEvent* event) override;
private slots:
    void childExpandChanged(QCtmDrawerItemWidget* item, bool expand);
    void doResize();
    void onItemExpand(bool expand);
    void onItemClicked(bool expand);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
