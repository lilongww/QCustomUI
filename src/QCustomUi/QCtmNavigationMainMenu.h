#pragma once

#include "qcustomui_global.h"

#include "QCtmNavigationSidePanel.h"

#include <memory>

class QCtmNavigationBackstageMenu ;
class QWidgetAction;

class QCUSTOMUI_EXPORT QCtmNavigationMainMenu : public QCtmNavigationSidePanel
{
	Q_OBJECT
        Q_PROPERTY(QSize itemSize READ itemSize WRITE setItemSize)
		Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
		Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)
        Q_PROPERTY(bool colorAlternate READ colorAlternate WRITE setColorAlternate)
public:
	QCtmNavigationMainMenu(QCtmNavigationBar *parent = nullptr);
	~QCtmNavigationMainMenu();

	/**
	 * @brief       添加菜单项
	 * @param[in]   text 菜单项名称
	 * @Return:     菜单项地址		
	 */
	QAction* addAction(const QString& text);

	/**
	 * @brief  		添加菜单项
	 * @param[in]  	icon 菜单项图标
	 * @param[in] 	text 菜单项名称
	 * @Return:   	菜单项地址		
	 */
	QAction* addAction(const QIcon& icon, const QString& text);

	/**
	 * @brief  	    添加分组菜单
	 * @param[in]   icon 菜单图标
	 * @param[in]   text 菜单文字
	 * @param[in]   menu 分组菜单地址
	 * @Return:     菜单项地址		
	 */
	QAction* addGroupingMenu(const QIcon& icon, const QString& text, QCtmNavigationBackstageMenu * menu);

	/**
	 * @brief       添加分组菜单
	 * @param[in] 	text 菜单文字
	 * @param[in] 	menu 分组菜单地址
	 * @Return:   	菜单项地址
	 */
	QAction* addGroupingMenu(const QString& text, QCtmNavigationBackstageMenu * menu);

	/**
	 * @brief  		插入分组菜单
	 * @param[in]  	index 插入位置
	 * @param[in]  	icon 菜单图标
	 * @param[in] 	text 菜单文字
	 * @param[in] 	menu 分组菜单地址
	 * @Return:   	菜单项地址
	 */
	QAction* insertGroupingMenu(int index, const QIcon& icon, const QString& text, QCtmNavigationBackstageMenu * menu);

	/**
     * @brief       插入分组菜单
     * @param[in]   index 插入位置
     * @param[in] 	text 菜单文字
     * @param[in] 	menu 分组菜单地址
     * @Return:   	菜单项地址
     */
	QAction* insertGroupingMenu(int index, const QString& text, QCtmNavigationBackstageMenu * menu);

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
	 * @brief       查询菜单项位置
	 * @param[in]   action 菜单项地址
	 * @Return:   	菜单项位置	
	 */
	int indexOf(QAction* action) const;

	/**
	 * @brief  		获取菜单项数量
	 * @Return:   	菜单项数量		
	 */
	int count()const;

	/**
     * @brief  		获取指定位置的action,如果该位置没有action则返回nullptr
     * @param[in]  	index 菜单位置
     * @Return:   	菜单地址	
     */
	QAction* actionAt(int index)const;

    /**
     * @brief		设置菜单项大小
     * @param[in]	size 大小
     */
    void setItemSize(const QSize& size);

    /**
     * @brief		获取菜单项大小
     */
    QSize itemSize() const;

	/**
	 * @brief		设置菜单项之间的水平间隔
	 * @param[in]	spacing 间隔像素
	 */
	void setHorizontalSpacing(int spacing);

	/**
	 * @brief		获取菜单项之间的水平间隔
	 */
	int horizontalSpacing() const;

	/**
	 * @brief		设置菜单项之间的垂直间隔
	 * @param[in]	spacing 间隔像素
	 */
	void setVerticalSpacing(int spacing);

	/**
	 * @brief		获取菜单项之间的垂直间隔
	 */
	int verticalSpacing() const;

    /**
     * @brief		设置是否使用颜色交替
     * @param[in]	enable true:启用，false:禁用
     */
    void setColorAlternate(bool enabled);

    /**
     * @brief	    是否使用颜色交替	
     */
    bool colorAlternate() const;
protected:
	bool event(QEvent* e) override;
    void actionEvent(QActionEvent *event) override;
	QSize sizeHint()const override;
	void insertAction(int index, QAction* action);
	void bindMenu(QAction* action, QCtmNavigationBackstageMenu * menu);

private slots:
	void relayout();
    void calcItemColor();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
