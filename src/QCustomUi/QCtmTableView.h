#pragma once

#include "qcustomui_global.h"

#include <QTableView>

class QCtmTableItemDelegate;

class QCUSTOMUI_EXPORT QCtmTableView : public QTableView
{
    Q_OBJECT

public:
    QCtmTableView(QWidget *parent = nullptr);
    ~QCtmTableView();

protected:
    bool viewportEvent(QEvent *event) override;
    virtual void setHoverIndex(const QModelIndex& index);
private:
    QCtmTableItemDelegate* m_delegate;
    QModelIndex m_hover;
};
