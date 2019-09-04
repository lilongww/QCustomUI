#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QWidgetAction>

#include <memory>

class QCUSTOMUI_EXPORT QCtmNavigationSideBar : public QWidget
{
	Q_OBJECT
        Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
	enum ActionPosition
	{
		Top,
		Bottom
	};
	QCtmNavigationSideBar(QWidget *parent);
	~QCtmNavigationSideBar();

	/**
	 * @brief       添加一个侧边栏按钮
	 * @param[in]   icon 按钮图标
	 * @param[in]   text 按钮文字，用于显示tooltip
	 * @param[in]   pos 停靠方向
	 * @Return:     侧边栏按钮地址
	 */
	QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);

    /**
     * @brief		添加一个action
     * @param[in]	action 要添加的action
     * @param[in]	pos action停靠方向
     */
    void addAction(QAction* action, ActionPosition pos);

	/**
     * @brief       插入一个侧边栏按钮
     * @param[in]   index 侧边栏按钮位置
     * @param[in]   icon 按钮图标
     * @param[in]   text 按钮文字，用于显示tooltip
     * @param[in]   pos 停靠方向
	 */
	QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);

    /**
     * @brief		插入一个action
     * @param[in]	index 插入位置
     * @param[in]	action 要插入的action
     * @param[in]   pos action停靠方向
     */
    void insertAction(int index, QAction* action, ActionPosition pos);

	/**
	 * @brief       获取侧边栏按钮位置，如果action不存在则返回-1
	 * @param[in]   action 侧边栏按钮
	 * @Return:     侧边栏按钮位置	
	 */
	int indexOf(QAction* action)const;

	/**
	 * @brief       获取按钮数量
	 * @param[in]   pos 停靠方向
	 * @Return:     按钮数量	
	 */
	int count(ActionPosition pos)const;

	/**
	 * @brief       设置图标显示大小
	 * @param[in]   size 图标的大小	
	 */
	void setIconSize(const QSize& size);

	/**
     * @brief       获取图标显示大小
     * @Return:     图标显示大小
	 */
	const QSize& iconSize()const;

    /**
     * @brief		查找指定位置的action
     * @param[in]	index 指定位置
     * @param[in]	pos action停靠方向
     * @Return:		action action地址
     */
    QAction* actionAt(int index, ActionPosition pos) const;

signals:
    /**
     * @brief		图标大小改变
     * @param[in]	size 图标大小
     */
    void iconSizeChanged(const QSize& size);
protected:
	void paintEvent(QPaintEvent *) override;
    void actionEvent(QActionEvent *event) override;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
