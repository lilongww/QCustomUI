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

#include <QWidget>

class QCtmStyleOptionSelectionButtonBox;
class QCUSTOMUI_EXPORT QCtmSelectionButtonBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(ExclusionPolicy exclusionPolicy READ exclusionPolicy WRITE setExclusionPolicy)
    Q_PROPERTY(bool uniformSize READ uniformSize WRITE setUniformSize)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
public:
    enum class ExclusionPolicy
    {
        None,
        Exclusive,
        ExclusiveOptional
    };
    Q_ENUM(ExclusionPolicy)
    QCtmSelectionButtonBox(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
    QCtmSelectionButtonBox(const QStringList& texts, Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
    ~QCtmSelectionButtonBox();
    void setExclusionPolicy(ExclusionPolicy policy);
    ExclusionPolicy exclusionPolicy() const;
    QString text(int index) const;
    QStringList texts() const;
    void setUniformSize(bool u);
    bool uniformSize() const;
    bool isChecked(int index) const;
    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation() const;
signals:
    void toggled(int index, bool checked);
public slots:
    void setText(int index, const QString& text);
    void remove(int index);
    void setTexts(const QStringList& texts);
    void setChecked(int index, bool checked = true);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private:
    std::vector<QRectF> calcSizes() const;
    void initStyleOption(int index, QCtmStyleOptionSelectionButtonBox& opt) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};