/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
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

#include "QCtmMaximumSizeButton_p.h"

#include <QStyleOption>
#include <QStylePainter>

QCtmMaximumSizeButton::QCtmMaximumSizeButton(QWidget* parent) : QToolButton(parent), m_maxsized(false) {}

QCtmMaximumSizeButton::~QCtmMaximumSizeButton() {}

void QCtmMaximumSizeButton::setMaximumSizedIcon(const QIcon& icon)
{
    m_maximumSizedIcon = icon;
    update();
}

const QIcon& QCtmMaximumSizeButton::maximumSizedIcon() const { return m_maximumSizedIcon; }

void QCtmMaximumSizeButton::setRevertIcon(const QIcon& icon)
{
    m_revertIcon = icon;
    update();
}

const QIcon& QCtmMaximumSizeButton::revertIcon() const { return m_revertIcon; }

void QCtmMaximumSizeButton::setMaximumSized(bool max)
{
    this->style()->unpolish(this);
    m_maxsized = max;
    this->style()->polish(this);
    emit maximumSizedChanged(max);
}

bool QCtmMaximumSizeButton::maximumSized() const { return m_maxsized; }

void QCtmMaximumSizeButton::initStyleOption(QStyleOptionToolButton* option) const
{
    QToolButton::initStyleOption(option);
    option->icon = m_maxsized ? m_maximumSizedIcon : m_revertIcon;
}

#if QT_VERSION_MAJOR < 6
void QCtmMaximumSizeButton::paintEvent(QPaintEvent* e)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}
#endif
