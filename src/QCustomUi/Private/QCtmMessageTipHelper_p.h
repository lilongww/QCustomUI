#pragma once

#include <QWidget>

#include <memory>

class QCtmMessageTipHelper : public QObject
{
	Q_OBJECT

public:
	QCtmMessageTipHelper(QWidget *parent);
	~QCtmMessageTipHelper();

	void drawMessageTip(int msgCount, QPainter& p, QWidget* w);

	void setTipColor(const QColor& color);
	const QColor& tipColor()const;
protected:
	QRect tipsRect(const QRect& rect);

private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
