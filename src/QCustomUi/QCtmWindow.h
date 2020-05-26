#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>

#include <memory>

namespace Ui
{
	class QCtmWindow;
}

class QCtmNavigationBar;
class QCtmTitleBar;

class QCUSTOMUI_EXPORT QCtmWindow : public QWidget
{
	Q_OBJECT

public:
	QCtmWindow(QWidget *parent = nullptr);
	~QCtmWindow();

	void setStatusBar(QStatusBar* statusBar);
	QStatusBar* statusBar() const;
	void removeStatusBar();
	void setMenuBar(QMenuBar* menuBar);
	QMenuBar* menuBar() const;
	void removeMenuBar();
	void setNavigationBar(QCtmNavigationBar* menuBar);
	QCtmNavigationBar* navigationBar()const;
	void removeNavigationBar();
	void setTitleBar(QCtmTitleBar* titleBar);
	QCtmTitleBar* titleWidget()const;
	void removeTitleBar();
	void setCentralWidget(QWidget *widget);
	QWidget* centralWidget()const;
#ifndef Q_OS_WIN
	void setShadowless(bool flag);
	bool shadowless() const;
#endif
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
	Ui::QCtmWindow* ui;
};
