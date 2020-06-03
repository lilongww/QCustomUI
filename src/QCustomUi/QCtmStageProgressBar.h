#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QPen>

#include <memory>

class QStyleOptionProgressBar;

class QCUSTOMUI_EXPORT QCtmStageProgressBar : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(int stageCount READ stageCount WRITE setStageCount)
		Q_PROPERTY(bool textVisible READ textVisible WRITE setTextVisible)
		Q_PROPERTY(QPen stageIndexTextColor READ stageIndexTextColor WRITE setStageIndexTextColor)
		Q_PROPERTY(QBrush rateBackground READ rateBackground WRITE setRateBackground)
public:
	QCtmStageProgressBar(QWidget *parent);
	~QCtmStageProgressBar();

	void setOrientation(Qt::Orientation orientation);
	Qt::Orientation orientation() const;

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

	void setStageIndexTextColor(const QPen& color);
	const QPen& stageIndexTextColor() const;

	void setRateBackground(const QBrush& brush);
	const QBrush& rateBackground() const;
protected:
	void paintEvent(QPaintEvent* event) override;
	virtual void drawStage(QPainter* p, int index, const QRectF& rect, int value) const;
	virtual void drawText(QPainter* p, int index, const QRectF& rect, const QString& text) const;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
	virtual void initStyleOption(QStyleOptionProgressBar* opt) const;
	int doMinimumWidth() const;
	int doMinimumHeigh() const;
	QRectF doStageRect(int index) const;
	QRectF doTextRect(int index) const;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
