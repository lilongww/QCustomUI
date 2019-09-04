#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QAction>

#include <memory>

class QCtmTabWidget;
class QCtmTabCornerLayout;

class QCUSTOMUI_EXPORT QCtmTabPage :public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief  		右上角添加一个action
	 * @param[in]  	icon 图标
	 * @param[in] 	text 文字
	 * @Return:   	action 地址
	 */
	QAction* addAction(const QIcon& icon, const QString& text);

	/**
     * @brief  		右上角添加一个action
     * @param[in]  	text 文字
     * @Return:   	action 地址
     */
	QAction* addAction(const QString& text);

	/**
     * @brief  		右上角插入一个action
     * @param[in]  	index 插入位置
     * @param[in] 	icon 图标
     * @param[in] 	text 文字
     * @Return:   	action 地址
     */
	QAction* insertAction(int index, const QIcon& icon, const QString& text);

	/**
	 * @brief  		右上角插入一个action
	 * @param[in]  	index 插入位置
	 * @param[in] 	text 文字
	 * @Return:   	action 地址
	 */
	QAction* insertAction(int index, const QString& text);

	/**
     * @brief      获取action数量
     * @Return:    action数量
     */
	int count() const;

    /**
     * @brief		插入action
     * @param[in]	index 要插入的位置
     * @param[in]	action 要插入的action	
     */
    void insertAction(int index, QAction* action);

    /**
     * @brief		获取指定位置的action
     * @param[in]	index 指定位置
     * @Return:		action地址
     */
    QAction* actionAt(int index) const;

	/**
	 * @brief		释放当前中央窗口
	 * @Return:		当前中央窗口地址
	 */
	QWidget* takeCentralWidget() const;

	/**
	 * @brief		中央窗口地址
	 */
	QWidget* centralWidget() const;

    using QWidget::addAction;
protected:
	void paintEvent(QPaintEvent *event) override;
    void actionEvent(QActionEvent* event) override;
	void setCentralWidget(QWidget* widget);

	QWidget* cornerWidget()const;
	QCtmTabPage(QCtmTabWidget* parent);
	~QCtmTabPage();
	friend QCtmTabWidget;
	friend QCtmTabCornerLayout;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
