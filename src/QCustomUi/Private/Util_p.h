#pragma once

#include <QAction>

#include <memory>

class QCtmWidgetItem;
typedef std::shared_ptr<QCtmWidgetItem> QCtmWidgetItemPtr;

namespace Util
{
    QCtmWidgetItemPtr find(QAction* action, const QList<QCtmWidgetItemPtr>& items);
    QCtmWidgetItemPtr find(QWidget* widget, const QList<QCtmWidgetItemPtr>& items);

    template <class Layout, class Item>
    void addItem(Item item, QList<Item>& items, QAction* before, Layout* layout)
    {
        auto it = std::find_if(items.begin(), items.end(), [=](Item item) { return item->action() == before; });
        it = items.insert(it, item);
        auto index = items.indexOf(*it);
        layout->insertWidget(index, item->widget());
        if (item->isCustomWidget())
        {
            item->widget()->show();
        }
    };

    template <class Layout, class Item>
    void removeItem(QAction* action, QList<Item>& items, Layout* layout)
    {
        auto item = Util::find(action, items);
        items.removeOne(item);
        layout->takeAt(layout->indexOf(item->widget()));
    }
}
