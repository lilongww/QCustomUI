#pragma once

#include <QStyledItemDelegate>

class QCtmClassifyTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QCtmClassifyTreeDelegate(QObject* parent);
    ~QCtmClassifyTreeDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
