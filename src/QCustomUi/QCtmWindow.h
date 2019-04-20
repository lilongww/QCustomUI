#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>

#include <memory>

namespace Ui
{
	class QCtmWindow;
}

class QCtmNavigationBar;
class QCtmTitleBar;

class QCUSTOMUI_EXPORT QCtmWindow : public QWidget
{
	Q_OBJECT

public:
	QCtmWindow(QWidget *parent = nullptr);
	~QCtmWindow();

	/**
	 * @brief  		设置状态栏，如果已存在statusbar，旧的statusbar将被销毁
	 * @param[in]  	statusBar 状态栏窗口
	 */
	void setStatusBar(QStatusBar* statusBar);
	
	/**
	 * @brief  		获取状态栏地址，如果没有设置statusbar则返回空
	 * @Return:   	状态栏地址	
	 */
	QStatusBar* statusBar() const;

	/**
	 * @brief  		移除状态栏，如果没有设置statusbar则不会产生任何效果。
	 */
	void removeStatusBar();

	/**
	 * @brief  		设置传统菜单栏
	 * @param[in]  	menuBar 菜单栏地址
	 */
	void setMenuBar(QMenuBar* menuBar);

	/**
	 * @brief  		获取传统菜单栏地址
	 * @Return:   	菜单栏地址		
	 */
	QMenuBar* menuBar() const;

	/**
	 * @brief  		移除传统菜单栏
	 * @note			
	 */
	void removeMenuBar();

	/**
	 * @brief  		设置导航菜单栏
	 * @param[in]  	menuBar 菜单栏地址
	 */
	void setNavigationBar(QCtmNavigationBar* menuBar);

	/**
	 * @brief  		获取导航菜单栏地址
	 * @Return:   	菜单栏地址		
	 */
	QCtmNavigationBar* navigationBar()const;

	/**
	 * @brief  		移除导航菜单栏		
	 */
	void removeNavigationBar();

	/**
     * @brief  		设置标题栏
     * @param[in]  	titleBar 标题栏地址
     */
	void setTitleBar(QCtmTitleBar* titleBar);

	/**
	 * @brief  		返回标题栏地址
	 * @Return:   	标题栏地址		
	 */
	QCtmTitleBar* titleWidget()const;

	/**
     * @brief  		移除标题栏
     */
	void removeTitleBar();

	/**
	 * @brief  		设置中心窗口
	 * @param[in]  	widget 中心窗口指针	
	 */
	void setCentralWidget(QWidget *widget);

	/**
	 * @brief  		获取中心窗口指针
	 * @Return:   	中心窗口指针	
	 */
	QWidget* centralWidget()const;

	/**
	 * @brief		设置窗口边框是否有阴影，如果窗口需要容纳OpenGL，则应设置为true
	 * @param[in]	flag true:无阴影，flase:有阴影
	 */
	void setShadowless(bool flag);

	/**
	 * @brief		窗口是否有阴影
	 */
	bool shadowless() const;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
	Ui::QCtmWindow* ui;
};
