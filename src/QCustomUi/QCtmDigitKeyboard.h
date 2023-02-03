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

#include "QCtmDialog.h"

#include <QVariant>

class QAbstractSpinBox;

class QCUSTOMUI_EXPORT QCtmDigitKeyboard : public QCtmDialog
{
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(QVariant minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(QVariant singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_OBJECT
public:
    enum class InputMode
    {
        IntInput = 1,
        DoubleInput
    };

    struct Unit
    {
        QString unit;
        QVariant minimum { 0 };
        QVariant maximum { 99 };
    };
    using Units = std::vector<Unit>;

    QCtmDigitKeyboard(QWidget* parent = nullptr);
    ~QCtmDigitKeyboard();

    void setInputMode(InputMode mode);
    InputMode inputMode() const;
    void setValue(const QVariant& value);
    QVariant value() const;
    void setMaximum(const QVariant& max);
    QVariant maximum() const;
    void setMinimum(const QVariant& min);
    QVariant minimum() const;
    void setRange(const QVariant& min, const QVariant& max);
    void setSingleStep(const QVariant& step);
    QVariant singleStep() const;
    void setUnits(const Units& units);
    const Units& units() const;
    void setCurrentUnitIndex(int index);
    int currentUnitIndex() const;
    void setDecimals(int decimals);
    int decimals() const;
    void bindBox(QAbstractSpinBox* box);
    static void simpleBindBox(QAbstractSpinBox* box, const Units& units = {}, const QVariant& step = {});
signals:
    void valueChanged(const QVariant& value);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void ensureConnect();
    void ensureConnect() const;
    void init();
    void clearUnits();
    void createUnits();
    void syncBindBox();
    void acceptUnit(int unitIndex);
    std::pair<QVariant, QString> showBefore();
    void showAfter(const QVariant& beforeValue, const QString& beforeUnit);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class QCtmDigitKeyboardInputHelper;
};
