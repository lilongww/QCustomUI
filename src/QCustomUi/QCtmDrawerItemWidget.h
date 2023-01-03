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

class QCtmDrawerWidget;

class QCUSTOMUI_EXPORT QCtmDrawerItemWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    void setWidget(QWidget* widget);
    QWidget* widget() const;
    void setTitle(const QString& title);
    QString title() const;
    bool isExpand() const;
    void setExpand(bool expand);
    QAction* addAction(const QIcon& icon, const QString& text);
    QAction* addAction(const QIcon& icon);
    QAction* addAction(const QString& text);
    QAction* insertAction(int index, const QIcon& icon, const QString& text);
    QAction* insertAction(int index, const QString& text);
    void removeAction(QAction* action);
    int count() const;
    int indexOf(QAction* action);
    QAction* actionAt(int index);
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void expandChanged(bool expand);
    void titleClicked(bool expand);
    void iconSizeChanged(const QSize& size);

protected:
    QCtmDrawerItemWidget(const QString& title, QCtmDrawerWidget* parent);
    ~QCtmDrawerItemWidget();

private slots:
    void onClicked();
    void resizeEvent(QResizeEvent* event);

private:
    int suggestSize() const;
    void setSuggestSize(int size);
    void insertAction(int index, QAction* action);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    friend class QCtmDrawerWidget;
};
