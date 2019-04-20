#pragma once

#include "qcustomui_global.h"

#include <QStyledItemDelegate>
#include <QTableView>

class QCUSTOMUI_EXPORT QCtmTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QCtmTableItemDelegate(QTableView* parent);
    ~QCtmTableItemDelegate();

    void setHoverIndex(const QModelIndex& index) { m_index = index; }
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QPersistentModelIndex m_index;
    QTableView* m_parent;
};
