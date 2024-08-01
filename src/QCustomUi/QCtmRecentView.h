/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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

#include <QTreeView>

class QCtmRecentModel;
struct QCtmRecentData;
class QCUSTOMUI_EXPORT QCtmRecentView : public QTreeView
{
    Q_PROPERTY(QIcon topButtonIcon READ topButtonIcon WRITE setTopButtonIcon)
    Q_PROPERTY(bool topButtonVisible READ topButtonVisible WRITE setTopButtonVisible)
    Q_OBJECT
public:
    explicit QCtmRecentView(QWidget* parent = nullptr);
    ~QCtmRecentView();
    virtual void setModel(QCtmRecentModel* model);
    QCtmRecentModel* model() const;
    void setTopButtonIcon(const QIcon& icon);
    QIcon topButtonIcon() const;
    void setTopButtonVisible(bool v);
    bool topButtonVisible() const;
signals:
    void topButtonClicked(const QModelIndex& index);
    void recentClicked(const QCtmRecentData& data);
    void recentDoubleClicked(const QCtmRecentData& data);

protected:
    void reset() override;
    virtual void onTopButtonClicked(const QModelIndex& index);
private slots:
    void onRowsRemoved(const QModelIndex& parent, int, int);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};