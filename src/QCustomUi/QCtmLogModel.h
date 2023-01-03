/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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

#pragma once

#include "QCtmAbstractLogModel.h"
#include "QCtmLogEvent.h"

#include <QDateTime>
#include <QIcon>

#include <memory>

struct QCUSTOMUI_EXPORT QCtmLogMessage
{
    QString msg;
    QDateTime dateTime;
    QtMsgType type;
};

class QCUSTOMUI_EXPORT QCtmLogModel : public QCtmAbstractLogModel
{
    Q_OBJECT
    Q_PROPERTY(QIcon infoIcon READ infoIcon WRITE setInfoIcon)
    Q_PROPERTY(QIcon warningIcon READ warningIcon WRITE setWarningIcon)
    Q_PROPERTY(QIcon errorIcon READ errorIcon WRITE setErrorIcon)
public:
    enum
    {
        TypeRole = Qt::UserRole + 1
    };

    QCtmLogModel(const QString& objectName, QObject* parent);
    ~QCtmLogModel();

    void clear() override;
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role /* = Qt::EditRole */) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    void setMaximumCount(int count);
    int maximumCount() const;
    void setLogInsertPolicy(QCtmLogData::LogInsertPolicy policy);
    QCtmLogData::LogInsertPolicy logInsertPolicy() const;
    void setInfoIcon(const QIcon& icon);
    const QIcon& infoIcon() const;
    void setWarningIcon(const QIcon& icon);
    const QIcon& warningIcon() const;
    void setErrorIcon(const QIcon& icon);
    const QIcon& errorIcon() const;
    int warningCount() const;
    int infoCount() const;
    int errorCount() const;

protected:
    void logEvent(QCtmLogEvent* e) override;
    void retranslateUi() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
