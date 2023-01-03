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

#include <QDialog>

#include <memory>

class QCtmTitleBar;

class QCUSTOMUI_EXPORT QCtmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QCtmDialog(QWidget* parent = nullptr);
    ~QCtmDialog();

    void setCentralWidget(QWidget* widget);
    QWidget* centralWidget() const;
    void setTitleBar(QCtmTitleBar* titleBar);
    QCtmTitleBar* titleBar() const;
    void addMoveBar(QWidget* moveBar);
    void removeMoveBar(QWidget* moveBar);
#ifndef Q_OS_WIN
    void setShadowless(bool flag);
    bool shadowless() const;
#endif
protected:
    void hideEvent(QHideEvent*) override;
    bool eventFilter(QObject*, QEvent*) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#endif
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
