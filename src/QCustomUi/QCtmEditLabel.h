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

#include <QIcon>
#include <QLabel>

class QStyleOptionToolButton;

class QCUSTOMUI_EXPORT QCtmEditLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QIcon editButtonIcon READ editButtonIcon WRITE setEditButtonIcon)
    Q_PROPERTY(QIcon finishButtonIcon READ finishButtonIcon WRITE setFinishButtonIcon)
    Q_PROPERTY(QIcon revertButtonIcon READ revertButtonIcon WRITE setRevertButtonIcon)
public:
    explicit QCtmEditLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit QCtmEditLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QCtmEditLabel();
    void setReadOnly(bool flag);
    bool isReadOnly() const;
    void setEditButtonIcon(const QIcon& icon);
    QIcon editButtonIcon() const;
    void setFinishButtonIcon(const QIcon& icon);
    QIcon finishButtonIcon() const;
    void setRevertButtonIcon(const QIcon& icon);
    QIcon revertButtonIcon() const;
signals:
    void editFinished();

protected:
    bool event(QEvent* e) override;
    QSize minimumSizeHint() const override;

private:
    void init();
private slots:
    void onEditButtonClicked();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
