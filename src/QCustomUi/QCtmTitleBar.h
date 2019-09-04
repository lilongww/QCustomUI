#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QMenu>

#include <memory>

namespace Ui
{
	class QCtmTitleBar;
}

class QCUSTOMUI_EXPORT QCtmTitleBar : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(bool showicon READ showIcon WRITE setShowIcon)
public:
	QCtmTitleBar(QWidget *parent = Q_NULLPTR);
	~QCtmTitleBar();

	/**
	 * @brief       设置传统菜单栏
	 * @param[in]   menu 传统菜单栏
	 * @note        如果已存在菜单栏，则旧的菜单栏将被销毁
	 */
	void setMenuBar(QMenuBar* menu);

	/**
	 * @brief  		获取传统菜单栏地址，如果没有设置菜单栏，则返回空
	 * @Return:   	传统菜单栏地址
	 */
	QMenuBar* menuBar()const;

	/**
	 * @brief  		移除传统菜单栏，如果菜单栏不存在不会产生任何效果。
	 */
	void removeMenuBar();

	/**
     * @brief  		设置是否显示标题栏图标
     * @param[in]  	show true:显示，false:不显示
     */
	void setShowIcon(bool show);

	/**
	 * @brief  		获取是否显示标题栏图标
	 * @Return:   	true:显示，false:不显示
	 */
	bool showIcon()const;

	private slots:
	void onCloseBtn();
	void onMaximumSizeBtn();
	void onMinimumSizeBtn();

protected:
	void paintEvent(QPaintEvent *event) override;
	void showEvent(QShowEvent* event) override;
	bool eventFilter(QObject *watched, QEvent *event)override;
	void actionEvent(QActionEvent *event) override;
private:
	Ui::QCtmTitleBar* ui;

	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
