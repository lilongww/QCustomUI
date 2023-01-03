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
#include <QWidget>

#include <memory>

class QCtmMessageTipButton;
class QCtmNavigationSidePane;

class QCUSTOMUI_EXPORT QCtmNavigationBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    enum ActionPosition
    {
        Left,
        Center,
        Right
    };
    QCtmNavigationBar(QWidget* parent);
    ~QCtmNavigationBar();

    void addAction(QAction* action, ActionPosition pos);
    QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);
    QAction* addAction(const QString& text, ActionPosition pos);
    void insertAction(int index, QAction* action, ActionPosition pos);
    QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);
    QAction* insertAction(int index, const QString& text, ActionPosition pos);
    QAction* addSeparator(ActionPosition pos);
    QAction* insertSeparator(int index, ActionPosition pos);
    void addPane(QAction* action, ActionPosition pos, QCtmNavigationSidePane* pane);
    QAction* addPane(const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane);
    QAction* addPane(const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane);
    void insertPane(int index, QAction* action, ActionPosition pos, QCtmNavigationSidePane* pane);
    QAction* insertPane(int index, const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane);
    QAction* insertPane(int index, const QString& text, ActionPosition pos, QCtmNavigationSidePane* pane);
    QAction* addHelp(const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);
    QAction* insertHelp(int index, const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);
    QAction* addLogo(const QIcon& icon, ActionPosition pos = Left);
    QAction* insertLogo(int index, const QIcon& icon, ActionPosition pos = Left);
    QAction* addUser(const QIcon& icon, const QString& text, ActionPosition pos = Right);
    QAction* insertUser(int index, const QIcon& icon, const QString& text, ActionPosition pos = Right);
    int indexOf(QAction* action) const;
    int count(ActionPosition pos) const;
    QRect actionRect(QAction* action);
    QAction* actionAt(int index, ActionPosition pos) const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);

protected:
    void actionEvent(QActionEvent* event) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
