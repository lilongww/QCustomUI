#pragma once

#include "qcustomui_global.h"

#include <QScrollArea>

#include <memory>

class QCtmNavigationBar;

class QCUSTOMUI_EXPORT QCtmNavigationSidePanel : public QWidget
{
	Q_OBJECT

public:
	enum class DockArea
	{
		Left,
		Top,
		Right,
		Bottom
	};
	explicit QCtmNavigationSidePanel(QCtmNavigationBar *parent = nullptr);
	~QCtmNavigationSidePanel();

	/**
	 * @brief  		设置停靠方向
	 * @param[in]  	area 停靠方向	
	 */
	void setDockArea(DockArea area);

	/**
     * @brief  		获取停靠方向
     * @Return:   	停靠方向
     */
	DockArea dockArea() const;

	/**
	 * @brief  		设置显示窗口，在调用前，必须先设置weiget的布局，在调用后设置的布局将无效。
	 * @param[in]  	widget 显示窗口地址
	 */
	void setWidget(QWidget* widget);

	/**
	 * @brief  		获取显示窗口的地址
	 * @Return:   	显示窗口的地址		
	 */
	QWidget* widget()const;

	/**
	 * @brief  		视图窗口容器
	 * @Return:   	视图窗口容器地址		
	 */
	QScrollArea* viewContainer()const;
	
	/**
	 * @brief  		设置标题栏可见
	 * @param[in]  	visbile 是否可见	
	 */
	void setTitleVisible(bool visible);

	/**
	 * @brief  		获取标题栏是否可见
	 * @Return:   	true 可见，false 不可见		
	 */
	bool titleVisible()const;

	/**
	 * @brief  		设置标题文字
	 * @param[in]  	text 标题文字		
	 */
	void setTitle(const QString& text);

	/**
	 * @brief  		获取标题文字
	 * @Return:   	标题文字		
	 */
	QString title()const;

    /**
     * @brief		设为弹出式窗口
     * @param[in]	popup 是否为弹出式窗口
     */
    void setPopup(bool popup);

    /**
     * @brief		获取窗口是否为弹出式窗口
     */
    bool popup() const;
signals:
	/**
	 * @brief  		面板关闭时发送该信号		
	 */
	void paneClosed();
protected:
	void paintEvent(QPaintEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	QSize sizeHint()const override;
	bool eventFilter(QObject* o, QEvent* e) override;
    void mousePressEvent(QMouseEvent *event) override;

	virtual QPoint smartPosition(DockArea area)const;
	virtual QSize smartSize(DockArea area)const;

    void setNavigationBar(QCtmNavigationBar* bar);
    QCtmNavigationBar* navigationBar() const;
private:
	void bindAction(QAction* action);
	void paintShadow(int shadowWidth);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	friend class QCtmNavigationBar;
};
