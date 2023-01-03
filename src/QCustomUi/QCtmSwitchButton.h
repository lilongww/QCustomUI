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

#include <QAbstractButton>

class QCUSTOMUI_EXPORT QCtmSwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QBrush uncheckedBackground READ uncheckedBackground WRITE setUncheckedBackground)
    Q_PROPERTY(QColor uncheckedBackgroundBorderColor READ uncheckedBackgroundBorderColor WRITE setUncheckedBackgroundBorderColor)
    Q_PROPERTY(QBrush uncheckedHandleBrush READ uncheckedHandleBrush WRITE setUncheckedHandleBrush)
    Q_PROPERTY(QColor uncheckedHandleBorderColor READ uncheckedHandleBorderColor WRITE setUncheckedHandleBorderColor)
    Q_PROPERTY(QBrush checkedBackground READ checkedBackground WRITE setCheckedBackground)
    Q_PROPERTY(QColor checkedBackgroundBorderColor READ checkedBackgroundBorderColor WRITE setCheckedBackgroundBorderColor)
    Q_PROPERTY(QBrush checkedHandleBrush READ checkedHandleBrush WRITE setCheckedHandleBrush)
    Q_PROPERTY(QColor checkedHandleBorderColor READ checkedHandleBorderColor WRITE setCheckedHandleBorderColor)
public:
    explicit QCtmSwitchButton(QWidget* parent);
    ~QCtmSwitchButton();

    void setUncheckedBackground(const QBrush& brush);
    const QBrush& uncheckedBackground() const;
    void setUncheckedBackgroundBorderColor(const QColor& pen);
    const QColor& uncheckedBackgroundBorderColor() const;
    void setUncheckedHandleBrush(const QBrush& brush);
    const QBrush& uncheckedHandleBrush() const;
    void setUncheckedHandleBorderColor(const QColor& pen);
    const QColor& uncheckedHandleBorderColor() const;
    void setCheckedBackground(const QBrush& brush);
    const QBrush& checkedBackground() const;
    void setCheckedBackgroundBorderColor(const QColor& pen);
    const QColor& checkedBackgroundBorderColor() const;
    void setCheckedHandleBrush(const QBrush& brush);
    const QBrush& checkedHandleBrush() const;
    void setCheckedHandleBorderColor(const QColor& pen);
    const QColor& checkedHandleBorderColor() const;

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
