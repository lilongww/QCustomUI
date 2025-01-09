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
#pragma once
#ifndef BUILD_STATIC

#include "qcustomui_global.h"

#include <QAbstractSpinBox>
#include <qtwidgetsglobal.h>

class QCtmULongLongSpinBoxPrivate;
class QCUSTOMUI_EXPORT QCtmULongLongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString cleanText READ cleanText)
    Q_PROPERTY(qulonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qulonglong maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qulonglong singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(StepType stepType READ stepType WRITE setStepType)
    Q_PROPERTY(qulonglong value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(int displayIntegerBase READ displayIntegerBase WRITE setDisplayIntegerBase)
    Q_PROPERTY(int displayFieldWidth READ displayFieldWidth WRITE setDisplayFieldWidth)
    Q_PROPERTY(QChar displayFillChar READ displayFillChar WRITE setDisplayFillChar)

public:
    explicit QCtmULongLongSpinBox(QWidget* parent = nullptr);
    ~QCtmULongLongSpinBox();
    qulonglong value() const;
    void setPrefix(const QString& prefix);
    QString prefix() const;
    void setSuffix(const QString& suffix);
    QString suffix() const;
    QString cleanText() const;
    void setSingleStep(qulonglong value);
    qulonglong singleStep() const;
    void setMinimum(qulonglong minimum);
    qulonglong minimum() const;
    void setMaximum(qulonglong maximum);
    qulonglong maximum() const;
    void setRange(qulonglong minimum, qulonglong maximum);
    void setStepType(QAbstractSpinBox::StepType stepType);
    StepType stepType() const;
    void setDisplayIntegerBase(int base);
    int displayIntegerBase() const;
    void setDisplayFieldWidth(int width);
    int displayFieldWidth() const;
    void setDisplayFillChar(QChar ch);
    QChar displayFillChar() const;
public slots:
    void setValue(qulonglong value);

signals:
    void valueChanged(qulonglong);
    void textChanged(const QString&);

protected:
    virtual QString textFromValue(qulonglong val) const;
    virtual qulonglong valueFromText(const QString& text) const;
    QValidator::State validate(QString& input, int& pos) const override;
    void fixup(QString& str) const override;
    QAbstractSpinBox::StepEnabled stepEnabled() const override;
    void stepBy(int steps) override;
    bool event(QEvent* event) override;

private:
    Q_DISABLE_COPY(QCtmULongLongSpinBox)
    Q_DECLARE_PRIVATE(QCtmULongLongSpinBox)
};
#endif