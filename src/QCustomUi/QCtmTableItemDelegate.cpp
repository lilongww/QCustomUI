#include "QCtmTableItemDelegate.h"

#include <QDebug>

QCtmTableItemDelegate::QCtmTableItemDelegate(QTableView *parent)
    : QStyledItemDelegate(parent)
    , m_parent(parent)
{
}

QCtmTableItemDelegate::~QCtmTableItemDelegate()
{
}

void QCtmTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    opt.state &= int(~QStyle::State_HasFocus);
    if (m_parent->selectionBehavior() == QAbstractItemView::SelectRows && index.row() == m_index.row())
    {
        opt.state |= QStyle::State_MouseOver;
    }
    else
        opt.state &= int(~QStyle::State_MouseOver);
	if (opt.features.testFlag(QStyleOptionViewItem::WrapText))
	{
		opt.features &= ~QStyleOptionViewItem::WrapText;
	}
    QStyledItemDelegate::paint(painter, opt, index);
}
