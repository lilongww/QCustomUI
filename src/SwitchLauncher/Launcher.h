#pragma once

#include "SWPluginInterface.h"

#include <QCustomUi/QCustomUi.h>

class SWPluginInterface;

class Launcher : public QCtmMainWindow
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = nullptr);
    ~Launcher();

    bool setCurrentActivity(SWPluginInterface* activity, const SWUserData& data = SWUserData());
	SWPluginInterface* currentActivity() const;

	void setThemes(const QStringList& themes);
	const QStringList& themes() const;

	void addMessageTip(QCtmAbstractMessagePtr msg);

	void setUser(const QString& user);

	void loadAlways();

    QString defaultStyleSheet(const QString& theme) const;

    QCtmLogWidget* logWidget() const;

	void setAboutDialog(QCtmDialog* about);
	QCtmDialog* aboutDialog() const;
signals:
	void themeChanged(const QString& theme);

protected:
    void showEvent(QShowEvent* e);
    void closeEvent(QCloseEvent* e);
    void initUi();
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
