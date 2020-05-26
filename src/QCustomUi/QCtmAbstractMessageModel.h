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

	 /** 
	  * @brief  添加一条消息
	  * @param  msg 消息内容	
	  */
	void addMessage(QCtmAbstractMessagePtr msg);

	 /** 
	  * @brief  插入一条消息
	  * @param  index 要插入的位置
	  * @param  msg 消息内容
	  */
	void insertMessage(int index, QCtmAbstractMessagePtr msg);

	 /** 
	  * @brief  移除一条消息
	  * @param  msg 消息内容
	  */
	void removeMessage(QCtmAbstractMessagePtr msg);

	 /** 
	  * @brief  获取指定行对应的消息内容
	  * @param  row 指定行
      * @return 指定行对应的消息内容
	  */
	QCtmAbstractMessagePtr message(int row)const;
	
	 /** 
	  * @brief    清除所有消息
	  */
	void clear();

	/**
	 * @brief		设置最大保存消息数量，默认1万条
	 * @param[in]	count 消息数量
	 */
	void setMaximumCount(int count);
	
	/**
	 * @brief		最大保存消息数量
	 */
	int maximumCount() const;

	int	rowCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override { return false; }
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool insertRows(int row, int count, const QModelIndex &parent  = QModelIndex()) override;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
