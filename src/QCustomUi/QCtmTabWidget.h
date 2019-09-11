#pragma once

#include "qcustomui_global.h"

#include <QTabWidget>
#include <QMap>
#include <QAction>

#include <memory>

class QCtmTabPage;

class QCUSTOMUI_EXPORT QCtmTabWidget :public QTabWidget
{
	Q_OBJECT
public:
	QCtmTabWidget(QWidget *parent = nullptr);
	~QCtmTabWidget();

public:
	/**
     * @brief       添加Tab页
     * @param[in]   widget	页窗口
     * @param[in]   label	页标签文本
     * @Return:     添加页的索引
     */
	QCtmTabPage* addTab(QWidget *widget, const QString &label);

	/**
	 * @brief		添加Tab页
	 * @param[in]	widget	页窗口
	 * @param[in]	icon	页标签图标
	 * @param[in]	label	页标签文本
	 * @Return:		添加页的索引
	 */
	QCtmTabPage* addTab(QWidget *widget, const QIcon &icon, const QString &label);

	/**
	 * @brief       插入Tab页
	 * @param[in]   index 插入页的index
	 * @param[in]   widget 页窗口
	 * @param[in]   label 页标签文本
	 * @Return:     插入页索引
	 */
	QCtmTabPage* insertTab(int index, QWidget *widget, const QString &label);

	/**
     *  @brief		插入Tab页
     *  @param[in]	index 插入页的index
     *  @param[in]	widget 页窗口
     *  @param[in]	icon 页标签图标
     *  @param[in]	label 页标签文本
     *  @Return:	插入页索引
     */
	QCtmTabPage* insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

signals:
    /**
     * @brief		隐藏按钮被点击
     */
	void closeButtonClicked();
    /**
     * @brief		位置调整按钮被点击
     */
	void areaButtonClicked();
protected:
	void paintEvent(QPaintEvent *e);

	private slots:
	void onCurrentChanged(int index);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

