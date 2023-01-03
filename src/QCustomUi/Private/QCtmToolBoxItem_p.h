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

#include <QIcon>
#include <QWidget>

#include "ui_QCtmToolBoxItem.h"

#include <memory>

class QCtmToolBoxItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    QCtmToolBoxItem(QWidget* parent);
    ~QCtmToolBoxItem();

    void setTitle(const QString& text);
    QString title() const;
    void setContent(QWidget* widget);
    QWidget* content() const;
    void setStretch(int stretch);
    int stretch() const;
    void setIcon(const QIcon& icon);
    const QIcon& icon() const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void actionEvent(QActionEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::QCtmToolBoxItem ui;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
