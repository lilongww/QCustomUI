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
#include "SelectionButtonBoxPropertySheet.h"

#include <QCustomUi/QCtmSelectionButtonBox.h>
#include <QtDesigner/private/formwindowbase_p.h>

struct SelectionButtonBoxPropertySheet::Impl
{
    QCtmSelectionButtonBox* box { nullptr };
    QString m_propertyGroup { "QCtmSelectionButtonBox" };
};

SelectionButtonBoxPropertySheet::SelectionButtonBoxPropertySheet(QObject* object, QObject* parent)
    : QDesignerPropertySheet(object, parent), m_impl(std::make_unique<Impl>(qobject_cast<QCtmSelectionButtonBox*>(object)))
{
    addDynamicProperty(CheckedIndexes, "0");                            // 添加动态属性checkedIndexesList
    setPropertyGroup(indexOf(CheckedIndexes), m_impl->m_propertyGroup); // 将属性分组到QCtmSelectionButtonBox组中
}

SelectionButtonBoxPropertySheet::~SelectionButtonBoxPropertySheet()
{
}

QVariant SelectionButtonBoxPropertySheet::property(int index) const
{
    if (index < 0 || index >= count())
        return QVariant();
    if (propertyName(index) == CheckedIndexes)
    {
        auto checkedIndexes = m_impl->box->checkedIndexes();
        QStringList checkedIndexesStrList;
        for (int checkedIndex : checkedIndexes)
            checkedIndexesStrList.append(QString::number(checkedIndex));
        return checkedIndexesStrList.join(",");
    }
    return QDesignerPropertySheet::property(index);
}
