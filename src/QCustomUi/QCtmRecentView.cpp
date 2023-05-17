#include "QCtmRecentView.h"
#include "QCtmRecentModel.h"
#include "QCtmRecentViewDelegate.h"

#include <QPainter>
#include <QScrollBar>

struct QCtmRecentView::Impl
{
    std::map<QModelIndex, QRect> layoutMap;
};

QCtmRecentView::QCtmRecentView(QWidget* parent) : QTreeView(parent), m_impl(std::make_unique<Impl>())
{
    setItemDelegate(new QCtmRecentViewDelegate(this));
}

QCtmRecentView::~QCtmRecentView() {}

void QCtmRecentView::setModel(QCtmRecentModel* model) { QAbstractItemView::setModel(model); }

QCtmRecentModel* QCtmRecentView::model() const { return qobject_cast<QCtmRecentModel*>(QAbstractItemView::model()); }
