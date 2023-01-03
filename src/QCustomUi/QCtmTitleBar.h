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

#include <QMenu>
#include <QWidget>

#include <memory>

namespace Ui
{
class QCtmTitleBar;
}

class QMenuBar;

class QCUSTOMUI_EXPORT QCtmTitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool iconIsVisible READ iconIsVisible WRITE setIconVisible)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    QCtmTitleBar(QWidget* parent = Q_NULLPTR);
    ~QCtmTitleBar();

    void setMenuBar(QMenuBar* menu);
    QMenuBar* menuBar() const;
    void setIconVisible(bool show);
    bool iconIsVisible() const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);
private slots:
    void onCloseBtn();
    void onMaximumSizeBtn();
    void onMinimumSizeBtn();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void actionEvent(QActionEvent* event) override;

private:
    QRect doIconRect() const;

private:
    Ui::QCtmTitleBar* ui;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
#ifdef Q_OS_WIN
    friend class QCtmWinFramelessDelegate;
#endif
};
