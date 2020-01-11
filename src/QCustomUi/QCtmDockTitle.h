#pragma once

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QCUSTOMUI_EXPORT QCtmDockTitle : public QWidget
{
	Q_OBJECT

public:
	QCtmDockTitle(QWidget *parent);
	~QCtmDockTitle();

protected:
	void paintEvent(QPaintEvent* event) override;
	void actionEvent(QActionEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
private:
	void init();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
