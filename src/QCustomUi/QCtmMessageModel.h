#pragma once

#include "qcustomui_global.h"

#include "QCtmAbstractMessageModel.h"

#include <QColor>

class QCUSTOMUI_EXPORT QCtmMessageModel : public QCtmAbstractMessageModel
{
	Q_OBJECT
		Q_PROPERTY(QColor titlecolor READ titleColor WRITE setTitleColor)
		Q_PROPERTY(QColor timecolor READ timeColor WRITE setTimeColor)
public:
	QCtmMessageModel(QObject *parent);
	~QCtmMessageModel();

	QVariant data(const QModelIndex& index, int role)const override;
	int columnCount(const QModelIndex &parent /* = QModelIndex() */)const override;

	/** 
	 * @brief   设置标题显示颜色
	 * @param   color 颜色
	 */
	void setTitleColor(const QColor& color);

	/** 
     * @brief	获取标题显示颜色
     * @return	颜色
     */
	const QColor& titleColor()const;

	/** 
	 * @brief	设置时间显示颜色
	 * @param	color 颜色
	 */
	void setTimeColor(const QColor& color);

	/** 
	 * @brief	获取时间显示颜色
	 * @return	颜色
	 */
	const QColor& timeColor()const;
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
