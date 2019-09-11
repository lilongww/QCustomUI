#pragma once

#include <memory>

#include "QCtmToolButton_p.h"

class QCtmUserButton : public QCtmToolButton
{
	Q_OBJECT
public:
	QCtmUserButton(QWidget *parent = nullptr);
	~QCtmUserButton();

protected:
	void paintEvent(QPaintEvent *) override;
	QSize sizeHint() const;
	QSize minimumSizeHint() const;
	QRect doTextRect() const;
	QRect doIconRect() const;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
