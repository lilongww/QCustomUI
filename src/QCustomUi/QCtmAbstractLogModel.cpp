#include "QCtmAbstractLogModel.h"
#include "QCtmLogEvent.h"
#include "QCtmLogManager.h"

QCtmAbstractLogModel::QCtmAbstractLogModel(const QString& objectName, QObject *parent)
    : QAbstractTableModel(parent)
{
    setObjectName(objectName);
    QCtmLogManager::instance().registerModel(this);
}

QCtmAbstractLogModel::~QCtmAbstractLogModel()
{
    QCtmLogManager::instance().unRegisterModel(this);
}

bool QCtmAbstractLogModel::insertRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool QCtmAbstractLogModel::insertColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginInsertColumns(parent, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool QCtmAbstractLogModel::removeRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginRemoveRows(parent, row, count + count - 1);
    endRemoveRows();
    return true;
}

bool QCtmAbstractLogModel::removeColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
}

void QCtmAbstractLogModel::logEvent(QCtmLogEvent* e)
{
    
}

bool QCtmAbstractLogModel::event(QEvent * e)
{
    if (e->type() == QCtmLogEvent::QCtmLog)
    {
        auto evt = dynamic_cast<QCtmLogEvent*>(e);
        if (evt)
        {
            logEvent(evt);
        }
    }
    else if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return false;
}
