#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QList>
#include <QMouseEvent>
#include <QMap>
#include <QScrollArea>
#include <QSplitter>

#include <memory>

class QCtmTreeItem;

class QCUSTOMUI_EXPORT QCtmTree : public QWidget
{
	Q_OBJECT
        Q_PROPERTY(bool exclusive READ exclusive WRITE setExclusive)
public:
	QCtmTree(QWidget *parent = nullptr);
	~QCtmTree();

	/** 
	 * @brief	    添加一个子窗口
	 * @param[in]	title 标题
     * @param[in]	widget 子窗口地址
     * @return:     item地址
	 */
    QCtmTreeItem* addWidget(const QString& title, QWidget* widget);

    /**
     * @brief	    插入一个子窗口
     * @param[in]	index 要插入的位置
     * @param[in]	title 标题
     * @param[in]	widget 子窗口地址
     * @return:     item地址
     */
    QCtmTreeItem* insertWidget(int index, const QString& title, QWidget* widget);

	/** 
	 * @brief	    移除一项
	 * @param[in]	item 要移除的项	
	 */
	void removeItem(QCtmTreeItem* item);

	/** 
	 * @brief	    获取项所在位置
	 * @param[in]	item 要获取位置的项
	 * @return: 	位置
	 */
	int indexOf(QCtmTreeItem* item) const;

	/** 
	 * @brief	    获取指定位置的项
	 * @param[in]	index 指定位置
	 * @return:	    项地址	
	 */
	QCtmTreeItem* item(int index) const;

	/** 
	 * @brief	    设置所有项的展开状态
	 * @param[in]	expand true:展开，false:收拢	
	 */
	void setAllExpand(bool expand) const;

	/** 
     * @brief	    获取项总数
     * @return: 	项总数
     */
	int count() const;

    /**
     * @brief		设置子项展开互斥，即只能有一项展开
     * @param[in]	
     */
    void setExclusive(bool exclusive);

    /**
     * @brief		子项是否展开互斥
     */
    bool exclusive() const;

    /**
     * @brief		设置每一个item的高度
     * @param[in]	sizes 每个item的高度
     */
    void setSizes(const QList<int>& sizes);
signals:
    /**
     * @brief		子项展开变化
     * @param[in]   item 展开状态发生变化的项
     * @param[in]	expand true:展开，false:关闭
     */
    void itemExpandChanged(QCtmTreeItem* item, bool expand);

    /**
     * @brief		标题栏点击事件
     * @param[in]	item 子项地址
     * @param[in]	expand true:展开，false:关闭
     */
    void itemTitleClicked(QCtmTreeItem* item, bool expand);
protected:
	bool allClosed()const;
	int total(const QList<int>& sizes) const;
    void showEvent(QShowEvent* event) override;
private slots:
	void childExpandChanged(QCtmTreeItem* item, bool expand, int size);
    void doResize();
    void onItemExpand(bool expand);
    void onItemClicked(bool expand);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

/*!***************************************************************************
* @class QCtmTreeItem
* @brief QCtmTree的子节点实现
*****************************************************************************/
class QCUSTOMUI_EXPORT QCtmTreeItem:public QWidget
{
	Q_OBJECT
public:

	/**
     * @brief  		设置显示窗口
     * @param[in]  	widget 窗口地址
	 */
	void setWidget(QWidget* widget);

	/**
	 * @brief  		获取显示窗口地址
	 * @Return:   	窗口地址
	 */
	QWidget* widget() const;

	/**
     * @brief  		设置叶子节点标题
     * @param[in]  	title 标题
     */
	void setTitle(const QString& title);

	/**
     * @brief  		获取叶子节点标题
     * @Return:   	标题
     */
	QString title() const;

	/**
     * @brief  		获取当前叶子节点是否展开
     * @Return:   	true:展开，false:关闭
     */
	bool isExpand()const;

	/**
	 * @brief  		设置当前叶子节点是否展开
	 * @param[in]  	expend true:展开，false:关闭
	 */
	void setExpand(bool expand);

    /**
     * @brief		在标题栏添加一个按钮
     * @param[in]	icon 按钮图标
     * @param[in]	text 按钮文字
     * @Return:		
     */
    QAction* addAction(const QIcon& icon, const QString& text);

    /**
    * @brief		在标题栏添加一个按钮
    * @param[in]	text 按钮文字
    * @Return:
    */
    QAction* addAction(const QString& text);

    /**
     * @brief		在标题栏插入一个按钮
     * @param[in]	index 要插入的位置
     * @param[in]   icon 按钮图标
     * @param[in]	text 按钮文字
     * @Return:		
     */
    QAction* insertAction(int index, const QIcon& icon, const QString& text);

    /**
     * @brief		在标题栏插入一个按钮
     * @param[in]	index 要插入的位置
     * @param[in]	text 按钮文字
     * @Return:
     */
    QAction* insertAction(int index, const QString& text);

    /**
     * @brief		从标题栏移除按钮
     * @param[in]	action 要移除的按钮
     */
    void removeAction(QAction *action);

    /**
     * @brief		标题按钮数量
     * @Return:		数量
     */
    int count() const;

    /**
     * @brief		获取action的位置
     * @param[in]	action 要获取位置的action
     * @Return:		位置
     */
    int indexOf(QAction* action);

    /**
     * @brief		获取指定位置的action
     * @param[in]	index 指定位置
     * @Return:		action地址
     */
    QAction* actionAt(int index);

signals:
    /**
     * @brief		展开状态改变
     * @param[in]	expand true:展开，false:关闭
     */
    void expandChanged(bool expand);

    /**
     * @brief		标题栏点击信号
     * @param[in]	expand true:展开，false:关闭
     */
    void titleClicked(bool expand);
protected:
    QCtmTreeItem(const QString& title, QCtmTree* parent);
    ~QCtmTreeItem();

	private slots:
	void onClicked();
	void resizeEvent(QResizeEvent *event);

private:
	int suggestSize()const;
    void setSuggestSize(int size);
    void insertAction(int index, QAction* action);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	friend QCtmTree;
};
