#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QMenu>

#include <memory>

namespace Ui
{
	class QCtmTitleBar;
}

class QMenuBar;

class QCUSTOMUI_EXPORT QCtmTitleBar : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(bool showicon READ showIcon WRITE setShowIcon)
public:
	QCtmTitleBar(QWidget *parent = Q_NULLPTR);
	~QCtmTitleBar();

	void setMenuBar(QMenuBar* menu);
	QMenuBar* menuBar()const;
	void removeMenuBar();
	void setShowIcon(bool show);
	bool showIcon()const;

	private slots:
	void onCloseBtn();
	void onMaximumSizeBtn();
	void onMinimumSizeBtn();

protected:
	void paintEvent(QPaintEvent *event) override;
	void showEvent(QShowEvent* event) override;
	bool eventFilter(QObject *watched, QEvent *event)override;
	void actionEvent(QActionEvent *event) override;

private:
	QRect doIconRect() const;
private:
	Ui::QCtmTitleBar* ui;

	struct Impl;
	std::unique_ptr<Impl> m_impl;
#ifdef Q_OS_WIN
	friend class QCtmWinFramelessDelegate;
#endif
};
