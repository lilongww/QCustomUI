#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QWidgetAction>

#include <memory>

class QCUSTOMUI_EXPORT QCtmNavigationSideBar : public QWidget
{
	Q_OBJECT
        Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
	enum ActionPosition
	{
		Top,
		Bottom
	};
	QCtmNavigationSideBar(QWidget *parent);
	~QCtmNavigationSideBar();

	QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);
    void addAction(QAction* action, ActionPosition pos);
	QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);
    void insertAction(int index, QAction* action, ActionPosition pos);
	int indexOf(QAction* action)const;
	int count(ActionPosition pos)const;
	void setIconSize(const QSize& size);
	const QSize& iconSize()const;
    QAction* actionAt(int index, ActionPosition pos) const;
signals:
    /**
     * @brief		图标大小改变
     * @param[in]	size 图标大小
     */
    void iconSizeChanged(const QSize& size);
protected:
	void paintEvent(QPaintEvent *) override;
    void actionEvent(QActionEvent *event) override;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
