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

#include <QToolButton>

class QCtmMaximumSizeButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool maximumsized READ maximumSized WRITE setMaximumSized NOTIFY maximumSizedChanged)
    Q_PROPERTY(QIcon maximumSizedIcon READ maximumSizedIcon WRITE setMaximumSizedIcon)
    Q_PROPERTY(QIcon revertIcon READ revertIcon WRITE setRevertIcon)

public:
    QCtmMaximumSizeButton(QWidget* parent);
    ~QCtmMaximumSizeButton();
    void setMaximumSizedIcon(const QIcon& icon);
    const QIcon& maximumSizedIcon() const;
    void setRevertIcon(const QIcon& icon);
    const QIcon& revertIcon() const;
    void setMaximumSized(bool max);
    bool maximumSized() const;
signals:
    void maximumSizedChanged(bool max);

protected:
    virtual void initStyleOption(QStyleOptionToolButton* option) const;

private:
    bool m_maxsized;
    QIcon m_maximumSizedIcon;
    QIcon m_revertIcon;
};
