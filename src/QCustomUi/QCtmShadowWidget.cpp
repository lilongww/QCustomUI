/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2026 LiLong                                              **
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
#include "QCtmShadowWidget.h"

#include <QGraphicsDropShadowEffect>

struct QCtmShadowWidget::Impl
{
    QGraphicsDropShadowEffect* shadowEffect = nullptr;
};

/*!
    \class      QCtmShadowWidget
    \brief      提供阴影效果的 QWidget 扩展.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmShadowWidget.h
*/

/*!
    \property   QCtmShadowWidget::shadowColor
    \brief      阴影颜色.
*/

/*!
    \property   QCtmShadowWidget::shadowRadius
    \brief      阴影模糊半径.
*/

/*!
    \property   QCtmShadowWidget::shadowOffset
    \brief      阴影偏移量.
*/

/*!
    \property   QCtmShadowWidget::shadowOffsetX
    \brief      阴影水平方向偏移量.
*/

/*!
    \property   QCtmShadowWidget::shadowOffsetY
    \brief      阴影垂直方向偏移量.
*/

/*!
    \property   QCtmShadowWidget::shadowEnabled
    \brief      阴影是否启用.
*/

/*!
    \fn         void QCtmShadowWidget::shadowColorChanged(const QColor& color)
    \brief      阴影颜色改变时发送该信号 \a color.
*/

/*!
    \fn         void QCtmShadowWidget::shadowRadiusChanged(qreal radius)
    \brief      阴影模糊半径改变时发送该信号 \a radius.
*/

/*!
    \fn         void QCtmShadowWidget::shadowOffsetChanged(const QPointF& offset)
    \brief      阴影偏移量改变时发送该信号 \a offset.
*/

/*!
    \fn         void QCtmShadowWidget::shadowOffsetXChanged(qreal offset)
    \brief      阴影水平方向偏移量改变时发送该信号 \a offset.
*/

/*!
    \fn         void QCtmShadowWidget::shadowOffsetYChanged(qreal offset)
    \brief      阴影垂直方向偏移量改变时发送该信号 \a offset.
*/

/*!
    \fn         void QCtmShadowWidget::shadowEnabledChanged(bool enabled)
    \brief      阴影启用状态改变时发送该信号 \a enabled.
*/

/*!
    \brief      构造函数 \a parent, \a f.
*/
QCtmShadowWidget::QCtmShadowWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), m_impl(std::make_unique<Impl>())
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    m_impl->shadowEffect = new QGraphicsDropShadowEffect(this);
    m_impl->shadowEffect->setColor(QColor(0, 0, 0, 100));
    m_impl->shadowEffect->setBlurRadius(10);
    m_impl->shadowEffect->setOffset(QPointF(0, 0));
    m_impl->shadowEffect->setEnabled(true);
    setGraphicsEffect(m_impl->shadowEffect);

    connect(m_impl->shadowEffect, &QGraphicsDropShadowEffect::colorChanged, this, &QCtmShadowWidget::shadowColorChanged);
    connect(m_impl->shadowEffect, &QGraphicsDropShadowEffect::blurRadiusChanged, this, &QCtmShadowWidget::shadowRadiusChanged);
    connect(m_impl->shadowEffect, &QGraphicsDropShadowEffect::offsetChanged, this, &QCtmShadowWidget::shadowOffsetChanged);
    connect(m_impl->shadowEffect, &QGraphicsDropShadowEffect::offsetChanged, this, [this](const QPointF& offset) {
        emit shadowOffsetXChanged(offset.x());
        emit shadowOffsetYChanged(offset.y());
    });
    connect(m_impl->shadowEffect, &QGraphicsEffect::enabledChanged, this, &QCtmShadowWidget::shadowEnabledChanged);
}

/*!
    \brief      析构函数.
*/
QCtmShadowWidget::~QCtmShadowWidget() = default;

/*!
    \brief      设置阴影颜色 \a color.
    \sa         shadowColor
*/
void QCtmShadowWidget::setShadowColor(const QColor& color)
{
    m_impl->shadowEffect->setColor(color);
}

/*!
    \brief      返回阴影颜色.
    \sa         setShadowColor
*/
QColor QCtmShadowWidget::shadowColor() const
{
    return m_impl->shadowEffect->color();
}

/*!
    \brief      设置阴影模糊半径 \a radius.
    \sa         shadowRadius
*/
void QCtmShadowWidget::setShadowRadius(qreal radius)
{
    m_impl->shadowEffect->setBlurRadius(radius);
}

/*!
    \brief      返回阴影模糊半径.
    \sa         setShadowRadius
*/
qreal QCtmShadowWidget::shadowRadius() const
{
    return m_impl->shadowEffect->blurRadius();
}

/*!
    \brief      设置阴影偏移量 \a offset.
    \sa         shadowOffset
*/
void QCtmShadowWidget::setShadowOffset(const QPointF& offset)
{
    m_impl->shadowEffect->setOffset(offset);
}

/*!
    \brief      返回阴影偏移量.
    \sa         setShadowOffset
*/
QPointF QCtmShadowWidget::shadowOffset() const
{
    return m_impl->shadowEffect->offset();
}

/*!
    \brief      设置阴影水平方向偏移量 \a offset.
    \sa         shadowOffsetX
*/
void QCtmShadowWidget::setShadowOffsetX(qreal offset)
{
    m_impl->shadowEffect->setXOffset(offset);
}

/*!
    \brief      返回阴影水平方向偏移量.
    \sa         setShadowOffsetX
*/
qreal QCtmShadowWidget::shadowOffsetX() const
{
    return m_impl->shadowEffect->xOffset();
}

/*!
    \brief      设置阴影垂直方向偏移量 \a offset.
    \sa         shadowOffsetY
*/
void QCtmShadowWidget::setShadowOffsetY(qreal offset)
{
    m_impl->shadowEffect->setYOffset(offset);
}

/*!
    \brief      返回阴影垂直方向偏移量.
    \sa         setShadowOffsetY
*/
qreal QCtmShadowWidget::shadowOffsetY() const
{
    return m_impl->shadowEffect->yOffset();
}

/*!
    \brief      设置阴影是否启用为 \a enabled.
    \sa         isShadowEnabled
*/
void QCtmShadowWidget::setShadowEnabled(bool enabled)
{
    m_impl->shadowEffect->setEnabled(enabled);
}

/*!
    \brief      返回阴影是否启用.
    \sa         setShadowEnabled
*/
bool QCtmShadowWidget::isShadowEnabled() const
{
    return m_impl->shadowEffect->isEnabled();
}
