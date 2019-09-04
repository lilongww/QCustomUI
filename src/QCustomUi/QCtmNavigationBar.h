#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QAction>

#include <memory>

class QCtmMessageTip;
class QCtmNavigationSidePanel;

class QCUSTOMUI_EXPORT QCtmNavigationBar : public QWidget
{
	Q_OBJECT

public:
	enum ActionPosition
	{
		Left,
		Right
	};
	QCtmNavigationBar(QWidget *parent);
	~QCtmNavigationBar();
	
	/**
	 * @brief       添加一个菜单项
	 * @param[in]   text 描述
	 * @param[in]   pos 停靠方向
	 * @return:     菜单项地址		
	 */
	QAction* addAction(const QString& text, ActionPosition pos);

	/**
	 * @brief  	    添加一个菜单项
	 * @param[in]   icon 菜单项图标
	 * @param[in] 	text 菜单描述
	 * @param[in]   pos 停靠方向
	 * @Return:   	菜单项地址	
	 */
	QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);

	/**
	 * @brief       添加菜单分隔项
	 * @param[in]   index 要插入的位置
	 * @Return:   	分隔项的地址	
	 */
	QAction* addSeparator(ActionPosition pos);

	/**
     * @brief  	    添加菜单分隔项
     * @param[in]   index 要插入的位置
     * @param[in] 	pos 停靠方向
     * @Return:
	 */
	QAction* insertSeparator(int index, ActionPosition pos);

	/**
	 * @brief  	    添加自定义弹出面板
	 * @param[in]   icon 菜单图标
	 * @param[in] 	text 菜单名称
	 * @param[in] 	pos 停靠方向
	 * @param[in] 	pane 自定义面板
	 * @Return:   	菜单项地址	
	 */
	QAction* addPane(const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);

	/**
	 * @brief  	    添加自定义弹出面板
	 * @param[in] 	text 菜单名称
	 * @param[in] 	pos 停靠方向
	 * @param[in] 	pane 自定义面板
	 * @Return:   	菜单项地址
	 */
	QAction* addPane(const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);

    /**
     * @brief		添加自定义弹出面板
     * @param[in]	action 与面板相绑定的按钮
     * @param[in]	pos 停靠方向
     * @Return:		
     */
    void addPane(QAction* action, ActionPosition pos, QCtmNavigationSidePanel* pane);

	/**
	 * @brief  	    插入菜单项
	 * @param[in]   index 菜单项位置
	 * @param[in] 	icon 菜单项图标
	 * @param[in] 	text 菜单项名称
	 * @param[in] 	pos 停靠方向
	 * @Return:   		
	 */
	QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);

	/**
	 * @brief  	    插入菜单项
	 * @param[in]   index 菜单项位置
	 * @param[in] 	text 菜单项名称
	 * @param[in] 	pos 停靠方向
	 * @Return:
	 */
	QAction* insertAction(int index, const QString& text, ActionPosition pos);

	/**
	 * @brief  	    插入自定义弹出面板
	 * @param[in]   index 菜单项位置
	 * @param[in] 	icon 菜单项图标
	 * @param[in] 	text 菜单项名称
	 * @param[in] 	pos 停靠方向
	 * @param[in] 	pane 弹出面板地址
	 * @Return:
	 */
	QAction* insertPane(int index, const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);

	/**
	 * @brief  	    插入自定义弹出面板
	 * @param[in]   index 菜单项位置
	 * @param[in] 	text 菜单项名称
	 * @param[in] 	pos 停靠方向
	 * @param[in] 	pane 弹出面板地址
	 * @Return:
	 */
	QAction* insertPane(int index, const QString& text, ActionPosition pos,  QCtmNavigationSidePanel* pane);

    /**
     * @brief  	    插入自定义弹出面板
     * @param[in]   index 菜单项位置
     * @param[in] 	action 与弹出面板绑定的按钮
     * @param[in] 	pos 停靠方向
     * @param[in] 	pane 弹出面板地址
     */
    void insertPane(int index, QAction* action, ActionPosition pos, QCtmNavigationSidePanel* pane);

	/**
	 * @brief  	    获取菜单项位置
	 * @param[in]   action 菜单项地址
	 * @Return:   	菜单项位置	
	 */
	int indexOf(QAction* action)const;

	/**
     * @brief  	    获取菜单项数量
     * @param[in]   pos 停靠方向
     * @Return:   	菜单项数量
	 */
	int count(ActionPosition pos)const;

	/**
	 * @brief  	    获取action的rect
	 * @param[in]   action action地址
	 * @Return:   	action的rect		
	 */
	QRect actionRect(QAction* action);

	/**
	 * @brief  	    添加帮助文档
	 * @param[in]   filePath 文件路径
     * @param[in]   icon 帮助按钮图标
     * @param[in]   pos 帮助按钮位置
	 */
	QAction* addHelp(const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);

	/**
	 * @brief  	    添加帮助文档
	 * @param[in]   index 插入位置
	 * @param[in]   filePath 文件路径
	 * @param[in]   icon 帮助按钮图标
	 * @param[in]   pos 帮助按钮位置
	 */
	QAction* insertHelp(int index, const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);

	/** 
	 * @brief		添加一个Logo按钮
	 * @param[in]	icon logo图标
	 * @param[in]	pos 图标停靠方向
	 * @return	
	 */
	QAction* addLogo(const QIcon& icon, ActionPosition pos = Left);


	/**
	 * @brief		插入一个Logo按钮
	 * @param[in]	index 插入位置
	 * @param[in]	icon logo图标
	 * @param[in]	pos 图标停靠方向
	 * @return
	 */
	QAction* insertLogo(int index, const QIcon& icon, ActionPosition pos = Left);

	QAction* addUser(const QIcon& icon, const QString& text, ActionPosition pos = Right);

	QAction* insertUser(int index, const QIcon& icon, const QString& text, ActionPosition pos = Right);

    /**
     * @brief		获取指定位置的action
     * @param[in]	index action的序号
     * @param[in]	pos action的停靠方向
     * @Return:		action地址
     */
    QAction* actionAt(int index, ActionPosition pos) const;

    /**
     * @brief		在指定位置插入一个action
     * @param[in]	index 要插入的位置
     * @param[in]	action 要插入的action地址
     * @param[in]   pos action停靠方向
     */
    void insertAction(int index, QAction* action, ActionPosition pos);

    /**
     * @brief		添加一个action
     * @param[in]	action 要添加的action
     * @param[in]	pos action停靠方向
     */
    void addAction(QAction* action, ActionPosition pos);
protected:
	void paintEvent(QPaintEvent* event)override;
    void actionEvent(QActionEvent *event) override;
	virtual void drawBackground(QPainter* p);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
