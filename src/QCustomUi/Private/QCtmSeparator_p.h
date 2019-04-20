#pragma once

#include <QWidget>
#include <QStyleOption>

class QCtmSeparator : public QWidget
{
	Q_OBJECT

public:
	QCtmSeparator(Qt::Orientation orientation, QWidget *parent = nullptr);
	~QCtmSeparator();

	Qt::Orientation orientation()const;
	
protected:
	QSize sizeHint() const override;
	void paintEvent(QPaintEvent*) override;
	void initStyleOption(QStyleOption* opt) const;

public slots:
	void setOrientation(Qt::Orientation orientation);

private:
	Qt::Orientation m_orientation;
};
