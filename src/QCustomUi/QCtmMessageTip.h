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

	void setModel(QCtmAbstractMessageModel* model);
	QCtmAbstractMessageModel* model()const;
	void setView(QCtmAbstractMessageView* view);
	QCtmAbstractMessageView* view()const;
	void setTipColor(const QColor& color);
	const QColor& tipColor()const;
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
