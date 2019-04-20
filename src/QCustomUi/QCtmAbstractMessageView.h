#pragma once

#include "qcustomui_global.h"
#include "QCtmNavigationSidePanel.h"

class QCtmAbstractMessageModel;
class QCtmNavigationBar;

class QCUSTOMUI_EXPORT QCtmAbstractMessageView : public QCtmNavigationSidePanel
{
	Q_OBJECT

public:
	QCtmAbstractMessageView(QCtmNavigationBar *parent);
	~QCtmAbstractMessageView();

	 /** 
	  * @brief  设置model
	  * @param  model 消息model	
	  */
	virtual void setModel(QCtmAbstractMessageModel* model) = 0;

	 /** 
	  * @brief  获取消息model
	  * @return 消息model
	  */
	virtual QCtmAbstractMessageModel* model()const = 0;

};
