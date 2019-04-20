#pragma once

#include "qcustomui_global.h"
#include "QCtmNavigationSidePanel.h"

#include <memory>

class QCtmNavigationBackstageActionGroup;
class QCtmNavigationMainMenu;

class QCUSTOMUI_EXPORT QCtmNavigationBackstageMenu  : public QCtmNavigationSidePanel
{
	Q_OBJECT
        Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor)
public:
	QCtmNavigationBackstageMenu(QCtmNavigationBar *parent = nullptr);
	~QCtmNavigationBackstageMenu();

	/**
	 * @brief       添加子菜单
	 * @param[in]   menu 子菜单地址
	 * @Return:   	
	 *	true        成功
	 *	false       失败，groupName重复	
	 */
	bool addSubMenu(QCtmNavigationBackstageActionGroup* menu);

	/**
	 * @brief       添加子菜单，如果该菜单名的groupName已存在直接返回此菜单，否则新建一个子菜单。
	 * @param[in]   groupName 子菜单分组名称
	 * @Return:   	子菜单地址	
	 */
	QCtmNavigationBackstageActionGroup* addSubMenu(const QString& groupName);

	/**
	 * @brief       移除子菜单
	 * @param[in]   groupName 子菜单名称		
	 */
	void removeSubMenu(const QString& groupName);

	/**
	 * @brief       移除子菜单
	 * @param[in]   menu 子菜单地址	
	 */
	void removeSubMenu(QCtmNavigationBackstageActionGroup* menu);

	/**
	 * @brief       插入子菜单
	 * @param[in]   index 子菜单位置
	 * @param[in] 	menu 子菜单地址
	 * @Return:		
	 *	true        成功
	 *	false       失败，存在重名	
	 */
	bool insertSubMenu(int index, QCtmNavigationBackstageActionGroup* menu);

	/**
	 * @brief       插入子菜单，如果该菜单名的groupName已存在直接返回此菜单，否则新建一个子菜单。
	 * @param[in]   index 子菜单位置
	 * @param[in] 	groupName 子菜单名称
	 * @Return:   	子菜单地址
	 */
	QCtmNavigationBackstageActionGroup* insertSubMenu(int index, const QString& groupName);

	/**
	 * @brief       获取子菜单的位置，如果子菜单不存在则返回-1
	 * @param[in]   groupName 子菜单名称
	 * @Return:   	子菜单位置
	 */
	int indexOf(const QString& groupName);

	/**
	 * @brief       获取子菜单数量
	 * @Return:     子菜单数量	
	 */
	int count()const;

	/**
	 * @brief       被主菜单调用显示
	 * @param[in]   mainMenu 主菜单地址	
	 */
	void showByMainMenu(QCtmNavigationMainMenu* mainMenu);

	/**
	 * @brief       获取index位置的菜单地址
	 * @param[in]   index 位置
	 * @Return:   	菜单地址
	 */
	QCtmNavigationBackstageActionGroup* subMenuAt(int index)const;

    /**
     * @brief		设置菜单上方边框颜色，用于与主菜单分隔
     * @param[in]	color 颜色
     */
    void setBaseColor(const QColor& color);

    /**
     * @brief		获取菜单上方边框颜色
     */
    const QColor& baseColor() const;
protected:
    void mousePressEvent(QMouseEvent *event) override;
	void closeEvent(QCloseEvent *event)override;
	QSize sizeHint() const override;
	QPoint smartPosition(DockArea align)const override;
	bool eventFilter(QObject* o, QEvent* e)override;
    void paintEvent(QPaintEvent *event) override;
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
