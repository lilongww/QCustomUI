#include "QCtmTableView.h"
#include "QCtmTableItemDelegate.h"

#include <QHeaderView>
#include <QHoverEvent>
#include <QPersistentModelIndex>

QCtmTableView::QCtmTableView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setObjectName("hHeader");
    verticalHeader()->setObjectName("vHeader");

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(24);

    m_delegate = new QCtmTableItemDelegate(this);
    setItemDelegate(m_delegate);
}

QCtmTableView::~QCtmTableView()
{
}

bool QCtmTableView::viewportEvent(QEvent *event)
{
    switch (event->type()) 
    {
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
        setHoverIndex(indexAt(static_cast<QHoverEvent*>(event)->pos()));
        break;
    case QEvent::HoverLeave:
    case QEvent::Leave:
        setHoverIndex(QModelIndex());
        break;
    case QEvent::Wheel:
        setHoverIndex(QModelIndex());
        break;
    default:
        break;
    }
    return QTableView::viewportEvent(event);
}

void QCtmTableView::setHoverIndex(const QModelIndex& index)
{
    if (!model())
        return;
    if (m_hover.isValid())
    {
        if (this->selectionBehavior() == QAbstractItemView::SelectRows)
        {
            for (int i = 0;i<this->model()->columnCount();i++)
            {
                auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(model()->index(m_hover.row(), i)));
                if(itd)
                    itd->setHoverIndex(QModelIndex());
            }
        }
        else
        {
            auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(m_hover));
            if(itd)
                itd->setHoverIndex(QModelIndex());
        }
    }
    m_hover = index;
    if (m_hover.isValid())
    {
        if (this->selectionBehavior() == QAbstractItemView::SelectRows)
        {
            for (int i = 0; i < this->model()->columnCount(); i++)
            {
                auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(model()->index(m_hover.row(), i)));
                if(itd)
                    itd->setHoverIndex(m_hover);
            }
        }
        else
        {
            auto itd = qobject_cast<QCtmTableItemDelegate*>(itemDelegate(m_hover));
            if(itd)
                itd->setHoverIndex(m_hover);
        }
    }
}
