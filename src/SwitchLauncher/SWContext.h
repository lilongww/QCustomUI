#pragma once

#include "SWPluginInterface.h"
#include "Launcher.h"

#include <memory>

//class Launcher;

class SWContext : public SWContextInterface
{
public:
    static SWContext& instance();

	Launcher* mainWindow() const override;
    
	SWPluginInterface* plugin(const QString& name) const override;
	QList<SWPluginInterface*> plugins() const override;
	QStringList activitiesNames() const override;
	bool setCurrentPlugin(SWPluginInterface* activity, const SWUserData &data = SWUserData()) override;
	void setThemeStyles(const QStringList& themes);
	const QStringList& themeStyles() const;
	void addMessageTip(QCtmAbstractMessagePtr msg) override;
	QAction* addLogo(const QIcon& icon, ActionPosition pos = 0) override;
	void setUserInfo(const QString& user, const QString& authority) override;
    void setMessageBarVisible(bool visible) override;
    bool messageBarVisible() const override;
	SWPluginInterface* currentPlugin() const override;
    void setLanguage(const QString& language) override;
    const QString& language() const override;
    void setAttributes(SWContextAttributes attributes) override;
	void setAttribute(SWContextAttribute attribute, bool enable) override;
    SWContextAttributes attributes() const override;
	void setMessageBarCurrentIndex(int index) override;
	int messageBarCurrentIndex() const override;
    QString defaultStyleSheet(const QString& theme) const override;
protected:
    SWContext();
    ~SWContext();
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};

