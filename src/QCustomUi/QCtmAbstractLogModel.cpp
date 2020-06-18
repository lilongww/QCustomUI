/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/

#include "QCtmAbstractLogModel.h"
#include "QCtmLogEvent.h"
#include "QCtmLogManager.h"

/*!
    \class      QCtmAbstractLogModel
    \brief      QCtmAbstractLogModel provide a abstract log model interface.
    \inherits   QAbstractTableModel
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a model with \a objectName and \a parent.
*/
QCtmAbstractLogModel::QCtmAbstractLogModel(const QString& objectName, QObject *parent)
    : QAbstractTableModel(parent)
{
    setObjectName(objectName);
    QCtmLogManager::instance().registerModel(this);
}

/*!
    \brief      Destroys the model.
*/
QCtmAbstractLogModel::~QCtmAbstractLogModel()
{
    QCtmLogManager::instance().unRegisterModel(this);
}

/*!
    \fn         void QCtmAbstractLogModel::clear()
    \brief      Clear the model.
*/

/*!
    \reimp
*/
bool QCtmAbstractLogModel::insertRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

/*!
    \reimp
*/
bool QCtmAbstractLogModel::insertColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginInsertColumns(parent, column, column + count - 1);
    endInsertColumns();
    return true;
}

/*!
    \reimp
*/
bool QCtmAbstractLogModel::removeRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginRemoveRows(parent, row, count + count - 1);
    endRemoveRows();
    return true;
}

/*!
    \reimp
*/
bool QCtmAbstractLogModel::removeColumns(int column, int count, const QModelIndex &parent /* = QModelIndex() */)
{
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
}

/*!
    \brief      Answer the log event, \a e.
*/
void QCtmAbstractLogModel::logEvent(QCtmLogEvent* e)
{
    Q_UNUSED(e)
}

/*!
    \reimp
*/
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
