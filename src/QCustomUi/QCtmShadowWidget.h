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
#pragma once

#include "qcustomui_global.h"

#include <QColor>
#include <QPointF>
#include <QWidget>

#include <memory>

class QCUSTOMUI_EXPORT QCtmShadowWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(qreal shadowRadius READ shadowRadius WRITE setShadowRadius NOTIFY shadowRadiusChanged)
    Q_PROPERTY(QPointF shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)
    Q_PROPERTY(qreal shadowOffsetX READ shadowOffsetX WRITE setShadowOffsetX NOTIFY shadowOffsetXChanged)
    Q_PROPERTY(qreal shadowOffsetY READ shadowOffsetY WRITE setShadowOffsetY NOTIFY shadowOffsetYChanged)
    Q_PROPERTY(bool shadowEnabled READ isShadowEnabled WRITE setShadowEnabled NOTIFY shadowEnabledChanged)
public:
    explicit QCtmShadowWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QCtmShadowWidget();
    void setShadowColor(const QColor& color);
    QColor shadowColor() const;
    void setShadowRadius(qreal radius);
    qreal shadowRadius() const;
    void setShadowOffset(const QPointF& offset);
    QPointF shadowOffset() const;
    void setShadowOffsetX(qreal offset);
    qreal shadowOffsetX() const;
    void setShadowOffsetY(qreal offset);
    qreal shadowOffsetY() const;
    void setShadowEnabled(bool enabled);
    bool isShadowEnabled() const;
 signals:
    void shadowRadiusChanged(qreal radius);
    void shadowColorChanged(const QColor& color);
    void shadowOffsetChanged(const QPointF& offset);
    void shadowOffsetXChanged(qreal offset);
    void shadowOffsetYChanged(qreal offset);
    void shadowEnabledChanged(bool enabled);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};