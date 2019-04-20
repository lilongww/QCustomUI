#pragma once

#include "qcustomui_global.h"

#include "QCtmAbstractMessageView.h"

#include <memory>

class QCtmNavigationBar;
class QCtmAbstractMessage;

typedef std::shared_ptr<QCtmAbstractMessage> QCtmAbstractMessagePtr;

class QCUSTOMUI_EXPORT QCtmMessageView : public QCtmAbstractMessageView
{
	Q_OBJECT
		Q_PROPERTY(QColor decoration READ decoration WRITE setDecoration)
		Q_PROPERTY(QColor titlecolor READ titleColor WRITE setTitleColor)
		Q_PROPERTY(QColor timecolor READ timeColor WRITE setTimeColor)
		Q_PROPERTY(QPixmap closebuttonicon READ closeButtonIcon WRITE setCloseButtonIcon)
public:
	QCtmMessageView(QCtmNavigationBar *parent);
	~QCtmMessageView();

	virtual void setModel(QCtmAbstractMessageModel* model);
	virtual QCtmAbstractMessageModel* model()const;

	/**
	 * @brief       设置装饰条颜色
	 * @param[in] 	color 颜色
	 */
	void setDecoration(const QColor& color);

	/**
     * @brief       获取装饰条颜色
     * @Return:   	颜色
     */
	const QColor& decoration()const;

	/**
     * @brief       设置标题文字颜色
     * @param[in]   color 颜色
     */
	void setTitleColor(const QColor& color);

	/**
     * @brief       获取标题文字颜色
     * @Return:   	颜色
     */
	const QColor& titleColor()const;

	/**
     *  @brief      设置时间文字颜色
     *  @param[in]  color 颜色
     */
	void setTimeColor(const QColor& color);

	/**
     * @brief       获取时间文字颜色
     * @Return:   	颜色
     */
	const QColor& timeColor()const;

	/**
     * @brief       设置消息关闭按钮图标
     * @param[in]   icon 图标
     */
	void setCloseButtonIcon(const QPixmap& icon);

	/**
     * @brief       获取关闭按钮图标
     * @Return:   	图标
     */
	const QPixmap& closeButtonIcon()const;
signals:
	void closeButtonClicked(const QModelIndex& index);
	void messageClicked(QCtmAbstractMessagePtr message);
protected:
	void resizeEvent(QResizeEvent*) override;
	void showEvent(QShowEvent*) override;
	bool eventFilter(QObject* o, QEvent* e) override;

	private slots:
	void onCloseButtonClicked(const QModelIndex& index);
	void onTitleClicked(const QModelIndex& index);
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
