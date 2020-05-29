#pragma once

#include "qcustomui_global.h"

#include <QScrollArea>

#include <memory>

class QCtmNavigationBar;

class QCUSTOMUI_EXPORT QCtmNavigationSidePanel : public QWidget
{
	Q_OBJECT

public:
	enum class DockArea
	{
		Left,
		Top,
		Right,
		Bottom
	};
	explicit QCtmNavigationSidePanel(QCtmNavigationBar *parent = nullptr);
	~QCtmNavigationSidePanel();

	void setDockArea(DockArea area);
	DockArea dockArea() const;
	void setWidget(QWidget* widget);
	QWidget* widget()const;
	QScrollArea* viewContainer()const;
	void setTitleVisible(bool visible);
	bool titleVisible()const;
	void setTitle(const QString& text);
	QString title()const;
    void setPopup(bool popup);
    bool popup() const;
signals:
	/**
	 * @brief  		面板关闭时发送该信号		
	 */
	void paneClosed();
protected:
	void paintEvent(QPaintEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	QSize sizeHint()const override;
	bool eventFilter(QObject* o, QEvent* e) override;
    void mousePressEvent(QMouseEvent *event) override;

	virtual QPoint smartPosition(DockArea area)const;
	virtual QSize smartSize(DockArea area)const;

    void setNavigationBar(QCtmNavigationBar* bar);
    QCtmNavigationBar* navigationBar() const;
private:
	void bindAction(QAction* action);
	void paintShadow(int shadowWidth);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	friend class QCtmNavigationBar;
};
