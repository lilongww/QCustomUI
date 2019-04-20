#include "Util_p.h"
#include "QCtmWidgetItem_p.h"



QCtmWidgetItemPtr Util::find(QAction* action, const QList<QCtmWidgetItemPtr>& items)
{
    for (auto &item : items)
    {
        if (item->action() == action)
            return item;
    }
    return nullptr;
}

QCtmWidgetItemPtr Util::find(QWidget* widget, const QList<QCtmWidgetItemPtr>& items)
{
    for (auto &item : items)
    {
        if (item->widget() == widget)
            return item;
    }
    return nullptr;
}
