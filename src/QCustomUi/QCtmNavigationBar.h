#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QAction>

#include <memory>

class QCtmMessageTip;
class QCtmNavigationSidePanel;

class QCUSTOMUI_EXPORT QCtmNavigationBar : public QWidget
{
	Q_OBJECT

public:
	enum ActionPosition
	{
		Left,
		Right
	};
	QCtmNavigationBar(QWidget *parent);
	~QCtmNavigationBar();

	QAction* addAction(const QString& text, ActionPosition pos);
	QAction* addAction(const QIcon& icon, const QString& text, ActionPosition pos);
	QAction* addSeparator(ActionPosition pos);
	QAction* insertSeparator(int index, ActionPosition pos);
	QAction* addPane(const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);
	QAction* addPane(const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);
    void addPane(QAction* action, ActionPosition pos, QCtmNavigationSidePanel* pane);
	QAction* insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos);
	QAction* insertAction(int index, const QString& text, ActionPosition pos);
	QAction* insertPane(int index, const QIcon& icon, const QString& text, ActionPosition pos, QCtmNavigationSidePanel* pane);
	QAction* insertPane(int index, const QString& text, ActionPosition pos,  QCtmNavigationSidePanel* pane);
    void insertPane(int index, QAction* action, ActionPosition pos, QCtmNavigationSidePanel* pane);
	int indexOf(QAction* action)const;
	int count(ActionPosition pos)const;
	QRect actionRect(QAction* action);
	QAction* addHelp(const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);
	QAction* insertHelp(int index, const QUrl& filePath, const QIcon& icon, ActionPosition pos = Right);
	QAction* addLogo(const QIcon& icon, ActionPosition pos = Left);
	QAction* insertLogo(int index, const QIcon& icon, ActionPosition pos = Left);
	QAction* addUser(const QIcon& icon, const QString& text, ActionPosition pos = Right);
	QAction* insertUser(int index, const QIcon& icon, const QString& text, ActionPosition pos = Right);
    QAction* actionAt(int index, ActionPosition pos) const;
    void insertAction(int index, QAction* action, ActionPosition pos);
    void addAction(QAction* action, ActionPosition pos);
protected:
	void paintEvent(QPaintEvent* event)override;
    void actionEvent(QActionEvent *event) override;
	virtual void drawBackground(QPainter* p);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
