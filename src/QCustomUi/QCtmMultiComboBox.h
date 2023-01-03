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

#include <QComboBox>

class QCUSTOMUI_EXPORT QCtmMultiComboBox : public QComboBox
{
    Q_OBJECT

public:
    QCtmMultiComboBox(QWidget* parent);
    ~QCtmMultiComboBox();

    QAbstractItemModel* model() const;

    QStringList checkedItems() const;
    QVariantList checkedDatas() const;

    void setChecked(int index, bool checked);
    bool isChecked(int index) const;

protected:
    void setModel(QAbstractItemModel* model);

private:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void showEvent(QShowEvent* e) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
