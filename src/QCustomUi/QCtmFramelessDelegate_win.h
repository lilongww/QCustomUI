#pragma once

#include <QWidget>

#include <memory>
#include <functional>

#ifdef Q_OS_WIN
class QCtmWinFramelessDelegate : public QObject
{
	Q_OBJECT

public:
	QCtmWinFramelessDelegate(QWidget* parent, const QWidgetList& moveBars);
	QCtmWinFramelessDelegate(QWidget* parent, QWidget* title);
	~QCtmWinFramelessDelegate();

	void addMoveBar(QWidget* w);
	void removeMoveBar(QWidget* w);

	bool nativeEvent(const QByteArray& eventType
		, void* message
		, long* &result);
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	void setWindowLong();
	void showSystemMenu(const QPoint& pos);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
#endif