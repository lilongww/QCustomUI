#pragma once

#include "qcustomui_global.h"

#include <QAbstractButton>

#include <memory>

class QCtmAbstractMessageModel;
class QCtmAbstractMessageView;
class QStyleOptionButton;

class QCUSTOMUI_EXPORT QCtmMessageTip : public QAbstractButton
{
	Q_OBJECT
		Q_PROPERTY(QColor tipcolor READ tipColor WRITE setTipColor)
public:
	QCtmMessageTip(QWidget *parent);
	~QCtmMessageTip();

	/**
     * @brief       设置数据来源
     * @param[in]   model 数据源
	 */
	void setModel(QCtmAbstractMessageModel* model);

	/**
	 * @brief       获取数据源
	 * @Return:     数据源
	 */
	QCtmAbstractMessageModel* model()const;

	/**
	 * @brief       设置消息显示视图
	 * @param[in] 	view 视图地址
	 */
	void setView(QCtmAbstractMessageView* view);

	/**
     * @brief       获取消息显示视图
     * @Return:   	视图地址
     *
     * @note
     */
	QCtmAbstractMessageView* view()const;

	/**
     * @brief       设置提示背景色
     * @param[in]   color 背景色
     */
	void setTipColor(QColor color);

	/**
     * @brief       获取提示背景色
     * @Return:   	背景色
     */
	QColor tipColor()const;
private:
	void paintEvent(QPaintEvent *event) override;
	QSize sizeHint()const override;
	void connectView();
	void initStyleOption(QStyleOptionButton* opt);

	private slots:
	void onClicked(bool);
	void onModelDataChanged();
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
