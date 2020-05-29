#pragma once

#include "qcustomui_global.h"

#include <QAbstractTableModel>

#include <memory>

using QCtmAbstractMessagePtr = std::shared_ptr<class QCtmAbstractMessage>;

class QCUSTOMUI_EXPORT QCtmAbstractMessageModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	QCtmAbstractMessageModel(QObject *parent);
	~QCtmAbstractMessageModel();

	void addMessage(QCtmAbstractMessagePtr msg);
	void insertMessage(int index, QCtmAbstractMessagePtr msg);
	void removeMessage(QCtmAbstractMessagePtr msg);
	QCtmAbstractMessagePtr message(int row)const;
	void clear();
	void setMaximumCount(int count);
	int maximumCount() const;

	int	rowCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override { return false; }
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool insertRows(int row, int count, const QModelIndex &parent  = QModelIndex()) override;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
