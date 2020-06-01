#pragma once

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QStyleOptionProgressBar;

class QCUSTOMUI_EXPORT QCtmStageProgressBar : public QWidget
{
	Q_OBJECT

public:
	QCtmStageProgressBar(QWidget *parent);
	~QCtmStageProgressBar();

	void setStageCount(int count);
	int stageCount() const;

	void setStageCricleRadius(int radius);
	int stageCricleRadius() const;

	void setText(int stage, const QString& text);
	QString text(int stage) const;

	void setTextVisible(bool flag);
	bool textVisible() const;

	void setValue(int value);
	int value() const;

	void setMaximum(int max);
	int maximum() const;

	void setMinimum(int min);
	int minimum() const;
protected:
	void paintEvent(QPaintEvent* event) override;
	virtual void drawStage(QPainter* p, int index, const QRectF& rect) const;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
	virtual void initStyleOption(QStyleOptionProgressBar* opt) const;
	int doWidth() const;
	QRectF doStageRect(int index) const;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
