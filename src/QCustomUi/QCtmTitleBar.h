/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2026 LiLong                                              **
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

#include "QCtmAbstractTitleBar.h"

#include <QMenu>
#include <QWidget>

#include <memory>

namespace Ui
{
class QCtmTitleBar;
}

class QMenuBar;

class QCUSTOMUI_EXPORT QCtmTitleBar : public QCtmAbstractTitleBar
{
    Q_OBJECT
    Q_PROPERTY(bool iconIsVisible READ iconIsVisible WRITE setIconVisible)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    explicit QCtmTitleBar(QWidget* parent = nullptr);
    ~QCtmTitleBar();

    void setMenuBar(QMenuBar* menu);
    QMenuBar* menuBar() const;
    void setIconVisible(bool show);
    bool iconIsVisible() const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void paintEvent(QPaintEvent* event) override;
    void actionEvent(QActionEvent* event) override;
    void onWindowMaximized(bool isMaximumSized) override;
    void onWindowMaximizeButtonHint(bool showMaximizeButton) override;
    void onWindowCloseButtonHint(bool showCloseButton) override;
    void onWindowMinimizeButtonHint(bool showMinimizeButton) override;
    void onWindowTitleChanged(const QString& title) override;
    bool showIconSystemMenu(const QPoint& pos) const override;

private:
    QRect doIconRect() const;

private:
    Ui::QCtmTitleBar* ui;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
