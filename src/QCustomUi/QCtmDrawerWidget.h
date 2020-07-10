/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QList>
#include <QMouseEvent>
#include <QMap>
#include <QScrollArea>
#include <QSplitter>

#include <memory>

class QCtmDrawerItemWidget;

class QCUSTOMUI_EXPORT QCtmDrawerWidget : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(bool exclusive READ exclusive WRITE setExclusive)
public:
    QCtmDrawerWidget(QWidget* parent = nullptr);
    ~QCtmDrawerWidget();

    QCtmDrawerItemWidget* addWidget(const QString& title, QWidget* widget);
    QCtmDrawerItemWidget* insertWidget(int index, const QString& title, QWidget* widget);
    void removeItem(QCtmDrawerItemWidget* item);
    int indexOf(QCtmDrawerItemWidget* item) const;
    QCtmDrawerItemWidget* item(int index) const;
    void setAllExpand(bool expand) const;
    int count() const;
    void setExclusive(bool exclusive);
    bool exclusive() const;
    void setSizes(const QList<int>& sizes);
signals:
    void itemExpandChanged(QCtmDrawerItemWidget* item, bool expand);
    void itemTitleClicked(QCtmDrawerItemWidget* item, bool expand);

public slots:
    void expandAll();
    void collapseAll();
protected:
    bool allClosed()const;
    int total(const QList<int>& sizes) const;
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
