#pragma once

#include "qcustomui_global.h"

#include <QFrame>

#include <memory>

class QCtmNavigationBackstageMenu ;
class QWidgetAction;

class QCUSTOMUI_EXPORT QCtmNavigationBackstageActionGroup : public QFrame
{
	Q_OBJECT

public:
	/**
	 * @brief       构造函数
	 * @param[in]   groupName 子菜单名称
	 * @param[in] 	parent 父菜单地址		
	 */
	QCtmNavigationBackstageActionGroup(const QString& groupName, QCtmNavigationBackstageMenu  *parent);
	~QCtmNavigationBackstageActionGroup();

	/**
	 * @brief       添加菜单项
	 * @param[in]   text 菜单项名称
	 * @Return:   	菜单项地址	
	 */
	QAction* addAction(const QString& text);

	/**
	 * @brief       添加菜单项
	 * @param[in]   icon 菜单项图标
	 * @param[in] 	text 菜单项名称		
	 */
	QAction* addAction(const QIcon& icon, const QString& text);
	
	/**
	 * @brief       修改子菜单名称
	 * @param[in]   groupName 新的子菜单名称
	 * @Return:   	
	 *	true        成功
	 *	false       失败，父菜单已存在该分组名称
	 */
	bool setGroupName(const QString& groupName);

	/**
	 * @brief       获取子菜单名称
	 * @Return:     子菜单名称
	 */
	QString groupName()const;

	/**
	 * @brief       插入菜单项
	 * @param[in]   index 菜单项位置
	 * @param[in] 	icon 菜单项图标
	 * @param[in] 	text 菜单项文字
	 * @Return:   	菜单项地址	
	 */
	QAction* insertAction(int index, const QIcon& icon, const QString& text);

	/**
	 * @brief       插入菜单项
	 * @param[in]   index 菜单项位置
	 * @param[in] 	text 菜单项文字
	 * @Return:   	菜单项地址
	 */
	QAction* insertAction(int index, const QString& text);

	/**
	 * @brief       获取菜单项位置
	 * @param[in]   action 菜单项地址
	 * @Return:   	菜单项位置	
	 */
	int indexOf(QAction* action)const;

	/**
	 * @brief       获取菜单项数量
	 * @Return:   	菜单项数量	
	 */
	int count()const;

	/**
	 * @brief       获取指定index位置的action,如果index所在位置的action不存在，返回nullptr
	 * @param[in]   index 位置
	 * @Return:   	action地址
	 */
	QAction* actionAt(int index)const;

    /**
     * @brief		插入action
     * @param[in]	index 要插入的位置
     * @param[in]	action action地址
     */
    void insertAction(int index, QAction* action);

protected:
    void actionEvent(QActionEvent *event) override;

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
