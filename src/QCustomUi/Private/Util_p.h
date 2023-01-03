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

#include <QAction>

#include <memory>

class QCtmWidgetItem;
typedef std::shared_ptr<QCtmWidgetItem> QCtmWidgetItemPtr;

namespace Util
{
QCtmWidgetItemPtr find(QAction* action, const QList<QCtmWidgetItemPtr>& items);
QCtmWidgetItemPtr find(QWidget* widget, const QList<QCtmWidgetItemPtr>& items);

template<class Layout, class Item>
void addItem(Item item, QList<Item>& items, QAction* before, Layout* layout)
{
    auto it    = std::find_if(items.begin(), items.end(), [=](Item item) { return item->action() == before; });
    it         = items.insert(it, item);
    auto index = items.indexOf(*it);
    layout->insertWidget(index, item->widget());
    if (item->isCustomWidget())
    {
        item->widget()->setVisible(item->action()->isVisible());
    }
};

template<class Layout, class Item>
void removeItem(QAction* action, QList<Item>& items, Layout* layout)
{
    auto item = Util::find(action, items);
    items.removeOne(item);
    layout->takeAt(layout->indexOf(item->widget()));
}
} // namespace Util
