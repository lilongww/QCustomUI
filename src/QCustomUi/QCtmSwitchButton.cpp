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
#include "QCtmSwitchButton.h"

#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <QVariantAnimation>

struct QCtmSwitchButton::Impl
{
    QBrush uncheckedBackground { 0xEEEEEE };
    QColor uncheckedBackgroundBorder { 0xE3E3E3 };
    QBrush uncheckedHandle { 0xFFFFFF };
    QColor uncheckedHandleBorder { 0xDEDEDE };
    QBrush checkedBackground { 0x326CF3 };
    QColor checkedBackgroundBorder { Qt::transparent };
    QBrush checkedHandle { 0xFFFFFF };
    QColor checkedHandleBorder { Qt::transparent };

    QVariantAnimation ani;

    QSize sizeHint;
};

/*!
    \class      QCtmSwitchButton
    \brief      状态切换按钮.
    \inherits   QAbstractButton
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmSwitchButton.h
*/

/*!
    \property   QCtmSwitchButton::uncheckedBackground
    \brief      未选中状态的背景画刷.
*/

/*!
    \property   QCtmSwitchButton::uncheckedBackgroundBorderColor
    \brief      未选中状态的背景边框颜色.
*/

/*!
    \property   QCtmSwitchButton::uncheckedHandleBrush
    \brief      未选中状态的 handle 画刷.
*/

/*!
    \property   QCtmSwitchButton::uncheckedHandleBorderColor
    \brief      未选中状态的 handle 边框颜色.
*/

/*!
    \property   QCtmSwitchButton::checkedBackground
    \brief      选中状态的背景画刷.
*/

/*!
    \property   QCtmSwitchButton::checkedBackgroundBorderColor
    \brief      选中状态的背景边框颜色.
*/

/*!
    \property   QCtmSwitchButton::checkedHandleBrush
    \brief      选中状态的 handle 画刷.
*/

/*!
    \property   QCtmSwitchButton::checkedHandleBorderColor
    \brief      选中状态的 handle 边框颜色.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmSwitchButton::QCtmSwitchButton(QWidget* parent) : QAbstractButton(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->ani.setDuration(100);
    connect(&m_impl->ani, &QVariantAnimation::valueChanged, this, qOverload<void>(&QWidget::update));
    setCheckable(true);
    QVariant v(m_impl->checkedBackgroundBorder);
    auto vv = v.toString();
    connect(this,
            &QCtmSwitchButton::toggled,
            this,
            [=](bool checked)
            {
                m_impl->ani.setDirection(checked ? QAbstractAnimation::Direction::Forward : QAbstractAnimation::Direction::Backward);
                m_impl->ani.start();
            });
}

/*!
    \brief      析构函数.
*/
QCtmSwitchButton::~QCtmSwitchButton() {}

void QCtmSwitchButton::setUncheckedBackground(const QBrush& brush)
{
    m_impl->uncheckedBackground = brush;
    update();
}

const QBrush& QCtmSwitchButton::uncheckedBackground() const { return m_impl->uncheckedBackground; }

void QCtmSwitchButton::setUncheckedBackgroundBorderColor(const QColor& pen)
{
    m_impl->uncheckedBackgroundBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::uncheckedBackgroundBorderColor() const { return m_impl->uncheckedBackgroundBorder; }

void QCtmSwitchButton::setUncheckedHandleBrush(const QBrush& brush)
{
    m_impl->uncheckedHandle = brush;
    update();
}

const QBrush& QCtmSwitchButton::uncheckedHandleBrush() const { return m_impl->uncheckedHandle; }

void QCtmSwitchButton::setUncheckedHandleBorderColor(const QColor& pen)
{
    m_impl->uncheckedHandleBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::uncheckedHandleBorderColor() const { return m_impl->uncheckedHandleBorder; }

void QCtmSwitchButton::setCheckedBackground(const QBrush& brush)
{
    m_impl->checkedBackground = brush;
    update();
}

const QBrush& QCtmSwitchButton::checkedBackground() const { return m_impl->checkedBackground; }

void QCtmSwitchButton::setCheckedBackgroundBorderColor(const QColor& pen)
{
    m_impl->checkedBackgroundBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::checkedBackgroundBorderColor() const { return m_impl->checkedBackgroundBorder; }

void QCtmSwitchButton::setCheckedHandleBrush(const QBrush& brush)
{
    m_impl->checkedHandle = brush;
    update();
}

const QBrush& QCtmSwitchButton::checkedHandleBrush() const { return m_impl->checkedHandle; }

void QCtmSwitchButton::setCheckedHandleBorderColor(const QColor& pen)
{
    m_impl->checkedHandleBorder = pen;
    update();
}

const QColor& QCtmSwitchButton::checkedHandleBorderColor() const { return m_impl->checkedHandleBorder; }

/*!
    \reimp
*/
void QCtmSwitchButton::paintEvent(QPaintEvent* e)
{
    QPixmap pixmap(this->width(), this->height());
    pixmap.fill(Qt::transparent);
    {
        QPainter p(&pixmap);
        p.setRenderHint(QPainter::Antialiasing);
        auto checked = this->isChecked();
        p.setPen(checked ? m_impl->checkedBackgroundBorder : m_impl->uncheckedBackgroundBorder);
        p.setBrush(checked ? m_impl->checkedBackground : m_impl->uncheckedBackground);
        auto roundRect = QRect(0, 0, width(), height());
        p.drawRoundedRect(roundRect, height() / 2, height() / 2);
        p.setPen(checked ? m_impl->checkedHandleBorder : m_impl->uncheckedHandleBorder);
        p.setBrush(checked ? m_impl->checkedHandle : m_impl->uncheckedHandle);
        p.drawEllipse(m_impl->ani.currentValue().toRect());
    }
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    p.drawPixmap(QRect(0, 0, width(), height()),
                 this->style()->generatedIconPixmap(isEnabled() ? QIcon::Mode::Normal : QIcon::Mode::Disabled, pixmap, &opt));
}

/*!
    \reimp
*/
void QCtmSwitchButton::resizeEvent(QResizeEvent* event)
{
    QAbstractButton::resizeEvent(event);
    m_impl->ani.setStartValue(QRect(1, 1, height() - 2, height() - 2));
    m_impl->ani.setEndValue(QRect(width() - height() + 1, 1, height() - 2, height() - 2));
}

/*!
    \reimp
*/
QSize QCtmSwitchButton::minimumSizeHint() const
{
    if (!m_impl->sizeHint.isEmpty())
        return m_impl->sizeHint;
    QString s       = QStringLiteral("XXXX");
    QFontMetrics fm = fontMetrics();
    QSize sz        = fm.size(Qt::TextShowMnemonic, s);
    int w           = sz.width();
    int h           = sz.height();
    QStyleOptionButton opt;
    opt.initFrom(this);

    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height
    m_impl->sizeHint = style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this);
    return m_impl->sizeHint;
}

/*!
    \reimp
*/
QSize QCtmSwitchButton::sizeHint() const { return minimumSizeHint(); }
