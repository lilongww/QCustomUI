#pragma once

#include "qcustomui_global.h"

#include <QString>
#include <QDateTime>

#include <memory>

class QCUSTOMUI_EXPORT QCtmAbstractMessage
{
public:
	QCtmAbstractMessage();
	virtual ~QCtmAbstractMessage() {}

};

class QCUSTOMUI_EXPORT QCtmMessage :public QCtmAbstractMessage
{
public:
	enum Column
	{
		Title,
		Content,
		Time,
		ColumnCount
	};

	/**
	 * @brief       构造函数
	 * @param[in]   title 消息标题
	 * @param[in] 	content 消息内容
	 * @param[in]	time 消息时间
	 */
	QCtmMessage(const QString& title, const QString& content, const QDateTime& time);
	~QCtmMessage();

	/**
     * @brief       设置消息标题
     * @param[in]   title 标题信息
     */
	void setTitle(const QString& title);

	/**
     * @brief       获取标题信息
     * @Return:     标题
     */
	const QString& title()const;

	/**
     * @brief       设置消息内容
     * @param[in]   content 消息内容
     */
	void setContent(const QString& content);

	/**
     * @brief       获取消息内容
     * @Return:     消息内容
     */
	const QString& content()const;

	/**
     * @brief       设置消息时间
     * @param[in]   time 消息时间
     */
	void setDateTime(const QDateTime& time);

	/**
     * @brief       获取消息时间
     * @Return:     时间
     */
	const QDateTime& dateTime()const;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

