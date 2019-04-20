/**
  * @file   QCtmFramelessDelegate.h
  * @brief	无边框窗口的委托实现
  * @author LiLong
  * @date   2018.11.16
  * @note   
  * @version 1.0
  */ 
#pragma once

#include <QWidget>
#include <memory>

/*!
 * \class QCtmFramelessDelegate
 *
 * \brief 
 *
 * 无边框窗口的委托实现
 *
 * \note 
 *
 * \author LiLong
 *
 * \version 1.0
 *
 * \date 2018.11.16
 *
 */
class QPlatformSurfaceEvent;
class QCtmFramelessDelegate : public QObject
{
	Q_OBJECT

public:
	/**
	  *  @brief  		构造函数
	  *  @param[in]  	parent 父窗口地址，父窗口必须为顶层窗口
	  *  @param[in] 	moveBars 用于鼠标拖动的窗口
	  *  @Return:   	
	  *
	  *  @note			
	  */
	QCtmFramelessDelegate(QWidget *parent, const QWidgetList& moveBars);

	/**
	  *  @brief  		构造函数重载
	  *  @param[in]  	parent 父窗口地址，父窗口必须为顶层窗口
	  *  @param[in] 	title 标题栏，用于鼠标拖拽移动
	  *  @Return:   	
	  *
	  *  @note			
	  */
	QCtmFramelessDelegate(QWidget *parent, QWidget* title);
	~QCtmFramelessDelegate();

    void addMoveBar(QWidget* widget);

    void removeMoveBar(QWidget* widget);

	void setShadowless(bool flag);
	bool shadowless() const;
protected:
	bool eventFilter(QObject* obj, QEvent* e) override;
	void resizeEvent(QResizeEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
	void paintEvent(QPaintEvent* e);
	void paintShadow(QPainter& p, int shadowWidth);
    void styleChangeEvent(QEvent* e);
    void platformSurfaceEvent(QPlatformSurfaceEvent* e);
private:
	void region(const QPoint &cursorGlobalPoint);
    void updateLayout();
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
