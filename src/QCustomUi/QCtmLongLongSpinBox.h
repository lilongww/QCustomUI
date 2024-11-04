/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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

#ifndef BUILD_STATIC

#include "qcustomui_global.h"

#include <QAbstractSpinBox>
#include <qtwidgetsglobal.h>

class QCtmLongLongSpinBoxPrivate;
class QCUSTOMUI_EXPORT QCtmLongLongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString cleanText READ cleanText)
    Q_PROPERTY(qlonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qlonglong maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qlonglong singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(StepType stepType READ stepType WRITE setStepType)
    Q_PROPERTY(qlonglong value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(int displayIntegerBase READ displayIntegerBase WRITE setDisplayIntegerBase)
    Q_PROPERTY(int displayFieldWidth READ displayFieldWidth WRITE setDisplayFieldWidth)
    Q_PROPERTY(QChar displayFillChar READ displayFillChar WRITE setDisplayFillChar)

public:
    explicit QCtmLongLongSpinBox(QWidget* parent = nullptr);
    ~QCtmLongLongSpinBox();
    qlonglong value() const;
    void setPrefix(const QString& prefix);
    QString prefix() const;
    void setSuffix(const QString& suffix);
    QString suffix() const;
    QString cleanText() const;
    void setSingleStep(qlonglong value);
    qlonglong singleStep() const;
    void setMinimum(qlonglong minimum);
    qlonglong minimum() const;
    void setMaximum(qlonglong maximum);
    qlonglong maximum() const;
    void setRange(qlonglong minimum, qlonglong maximum);
    void setStepType(QAbstractSpinBox::StepType stepType);
    StepType stepType() const;
    void setDisplayIntegerBase(int base);
    int displayIntegerBase() const;
    void setDisplayFieldWidth(int width);
    int displayFieldWidth() const;
    void setDisplayFillChar(QChar ch);
    QChar displayFillChar() const;
public slots:
    void setValue(qlonglong value);

signals:
    void valueChanged(qlonglong);
    void textChanged(const QString&);

protected:
    virtual QString textFromValue(qlonglong value) const;
    virtual qlonglong valueFromText(const QString& text) const;
    QValidator::State validate(QString& input, int& pos) const override;
    void fixup(QString& str) const override;
    QAbstractSpinBox::StepEnabled stepEnabled() const override;
    void stepBy(int steps) override;
    bool event(QEvent* event) override;

private:
    Q_DISABLE_COPY(QCtmLongLongSpinBox)
    Q_DECLARE_PRIVATE(QCtmLongLongSpinBox)
};
#endif