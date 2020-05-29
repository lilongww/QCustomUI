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
	void setDecoration(const QColor& color);
	const QColor& decoration()const;
	void setTitleColor(const QColor& color);
	const QColor& titleColor()const;
	void setTimeColor(const QColor& color);
	const QColor& timeColor()const;
	void setCloseButtonIcon(const QPixmap& icon);
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
	std::unique_ptr<Impl> m_impl;
};
