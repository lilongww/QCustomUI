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

#include <memory>

class QCtmToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(bool showToolTips READ showToolTips WRITE setShowToolTips)
public:
    QCtmToolButton(QWidget* parent);
    ~QCtmToolButton();

    void setIcon(const QIcon& icon);
    void setShowToolTips(bool show);
    bool showToolTips() const;
    void setSelected(bool select);

protected:
    bool event(QEvent* e) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void initStyleOption(QStyleOptionToolButton* option) const override;
#else
    void paintEvent(QPaintEvent*) override;
#endif

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
