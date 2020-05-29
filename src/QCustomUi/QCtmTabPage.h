#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QAction>

#include <memory>

class QCtmTabWidget;
class QCtmTabCornerLayout;

class QCUSTOMUI_EXPORT QCtmTabPage :public QWidget
{
	Q_OBJECT
public:
	QAction* addAction(const QIcon& icon, const QString& text);
	QAction* addAction(const QString& text);
	QAction* insertAction(int index, const QIcon& icon, const QString& text);
	QAction* insertAction(int index, const QString& text);
	int count() const;
    void insertAction(int index, QAction* action);
    QAction* actionAt(int index) const;
	QWidget* takeCentralWidget() const;
	QWidget* centralWidget() const;

    using QWidget::addAction;
protected:
	void paintEvent(QPaintEvent *event) override;
    void actionEvent(QActionEvent* event) override;
	void setCentralWidget(QWidget* widget);

	QWidget* cornerWidget()const;
	QCtmTabPage(QCtmTabWidget* parent);
	~QCtmTabPage();
	friend QCtmTabWidget;
	friend QCtmTabCornerLayout;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
