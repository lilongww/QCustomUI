#pragma once

#include "qcustomui_global.h"

#include <QDialog>

#include <memory>

class QCtmTitleBar;

class QCUSTOMUI_EXPORT QCtmDialog : public QDialog
{
	Q_OBJECT

public:
	QCtmDialog(QWidget *parent = nullptr);
	~QCtmDialog();

	/**
	 * @brief       设置客户区，如果客户区窗口已存在，前一个客户区窗口将被销毁
	 * @param[in]   widget 客户区窗口	
	 */
	void setContent(QWidget* widget);

	/**
	 * @brief       获取客户区窗口地址，如果没有设置客户区窗口，默认存在一个QWidget
	 * @Return:     客户区窗口地址	
	 */
	QWidget* content()const;

	/**
	 * @brief       设置在父窗口中央位置显示
	 * @param[in]   showInCenter true:在中央位置显示，false:不在中央位置显示
	 */
	void setShowInCenter(bool showInCenter);

	/**
	 * @brief       是否在父窗口中央位置显示
	 */
	bool showInCenter() const;

    /**
     * @brief		获取标题栏地址
     */
    QCtmTitleBar* titleBar() const;

    /**
     * @brief		设置可拖动的控件
     * @param[in]	moveBars 可拖动的控件
     */
    void setMoveBars(const QWidgetList& moveBars);

    /**
     * @brief		移除可拖动控件
     * @param[in]	
     */
    void removeMoveBar(QWidget* moveBar);

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
	void showEvent(QShowEvent *) override;
	void hideEvent(QHideEvent *event) override;
private:
	void normalizes(QPoint& pos);

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
