#pragma once

#include "qcustomui_global.h"

#include <QTabWidget>
#include <QMap>
#include <QAction>

#include <memory>

class QCtmTabPage;

class QCUSTOMUI_EXPORT QCtmTabWidget :public QTabWidget
{
	Q_OBJECT
public:
	QCtmTabWidget(QWidget *parent = nullptr);
	~QCtmTabWidget();

public:
	QCtmTabPage* addTab(QWidget *widget, const QString &label);
	QCtmTabPage* addTab(QWidget *widget, const QIcon &icon, const QString &label);
	QCtmTabPage* insertTab(int index, QWidget *widget, const QString &label);
	QCtmTabPage* insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

signals:
    /**
     * @brief		隐藏按钮被点击
     */
	void closeButtonClicked();
    /**
     * @brief		位置调整按钮被点击
     */
	void areaButtonClicked();
protected:
	void paintEvent(QPaintEvent *e);

private slots:
	void onCurrentChanged(int index);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

