#pragma once

#include "qcustomui_global.h"

#include <QAbstractTableModel>
#include <QApplication>

#include <memory>

class QCtmLogEvent;

class QCUSTOMUI_EXPORT QCtmAbstractLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCtmAbstractLogModel(const QString& objectName, QObject *parent);
    ~QCtmAbstractLogModel();
    
    /**
     * @brief		清空所有数据
     */
    virtual void clear() = 0;

    bool insertRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */) override;
    bool insertColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */) override;
    bool removeRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */) override;
    bool removeColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */) override;
protected:
    bool event(QEvent* e) override;
    virtual void logEvent(QCtmLogEvent* e);
    virtual void retranslateUi() {};
};
