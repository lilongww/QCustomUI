#pragma once

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QCUSTOMUI_EXPORT QCtmToolBox :public QWidget
{
	Q_OBJECT

public:
	QCtmToolBox(QWidget* parent);
	~QCtmToolBox();

	/**
	 * @brief      添加一个子窗口
	 * @param[in]  title 标题
	 * @param[in] 	widget 子窗口地址
	 */
	void addWidget(const QString& title, QWidget* widget);

	/**
	 * @brief      插入一个子窗口
	 * @param[in]  index 插入位置
	 * @param[in] 	title 标题
	 * @param[in] 	widget 子窗口地址
	 */
	void insertWidget(int index, const QString& title, QWidget* widget);

	/**
	 * @brief  		移除子窗口
	 * @param[in]  	widget 子窗口地址
	 */
	void removeWidget(QWidget* widget);

	/**
     * @brief  		移除子窗口
     * @param[in]  	index 子窗口位置
     */
	void removeWidget(int index);

	/**
     * @brief  		查询子窗口位置
     * @param[in]  	widget 子窗口地址
     * @Return:   	子窗口位置
     */
	int indexOf(QWidget* widget) const;

	/**
	 * @brief  		查询指定位置的子窗口
	 * @param[in]  	index 指定位置
	 * @Return:   	子窗口地址
	 */
	QWidget* widget(int index) const;

	/**
     * @brief  		设置子窗口间默认显示比例
     * @param[in]  	index 子窗口位置
     * @param[in] 	stretch 子窗口显示比重
     */
	void setStretchFactor(int index, int stretch);

	/**
     * @brief  		设置各子窗口的显示大小(高度)
     * @param[in]  	sizes 从上到下的大小集合
     */
	void setSizes(const QList<int>& sizes);

	/**
	 *  @brief      设置窗口第一次显示时的比例
	 *  @param[in]  index 窗口序号
	 *  @param[in] 	stretch 比例
	 */
	void setDefaultStretch(int index, int stretch);
protected:
	void showEvent(QShowEvent *event) override;

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};