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

#include "QCtmLogEvent.h"
#include "qcustomui_global.h"

#include <QIcon>
#include <QWidget>

#include <memory>

class QCUSTOMUI_EXPORT QCtmLogWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QIcon infoIcon READ infoIcon WRITE setInfoIcon)
    Q_PROPERTY(QIcon warningIcon READ warningIcon WRITE setWarningIcon)
    Q_PROPERTY(QIcon errorIcon READ errorIcon WRITE setErrorIcon)
public:
    enum class QCtmLogColumn
    {
        Level,
        Time,
        Description
    };

    QCtmLogWidget(const QString& objectName, QWidget* parent = Q_NULLPTR);
    ~QCtmLogWidget();

    void setLogInsertPolicy(QCtmLogData::LogInsertPolicy policy);
    QCtmLogData::LogInsertPolicy logInsertPolicy() const;
    void setColumnVisible(QCtmLogColumn column, bool visible);
    bool columnVisible(QCtmLogColumn column) const;
    void setInfoIcon(const QIcon& icon);
    const QIcon& infoIcon() const;
    void setWarningIcon(const QIcon& icon);
    const QIcon& warningIcon() const;
    void setErrorIcon(const QIcon& icon);
    const QIcon& errorIcon() const;
    void setMaximumCount(int count);
    int maximumCount() const;

private:
    void init();
    void changeEvent(QEvent* e) override;
private slots:
    void updateLogCount();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
