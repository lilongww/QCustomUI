#pragma once

#include "editactivity_global.h"
#include "SwitchLauncher/SWPluginInterface.h"

#include <memory>

class EDITACTIVITY_EXPORT EditActivity
    : public QObject
    , public SWPluginInterface
	, public SWPluginSlots
{
    Q_OBJECT
        Q_PLUGIN_METADATA(IID "QCustomUi.SwitchLauncher.PluginInterface")
        Q_INTERFACES(SWPluginInterface)
public:
    EditActivity();
    ~EditActivity();

    int id() const override;
    QIcon icon() const override;
    QString name() const override;
    bool initialize(SWContextInterface* context) override;
    void release() override;
    QString styleSheet(const QString& theme) const override;

	bool currentPluginChangeEvent(bool current, const SWPluginInterface* old, const SWUserData& data = SWUserData()) override;
	void allActivitiesInitialized()override{}
    QWidget* content() const override;
    QCtmNavigationSideBar* sideBar() const override;
    QWidget* subSideBar() const override;
    QList<ActionInfo> navigationBarActions() const override;
	QList<TabPageInfo> messageBarTabPages() const override;
	QList<StatusBarInfo> statusBarWidgets() const override;
	QList<MenuInfo> subMenus() const override;
	void onMenuBarActionTriggered(QAction* action) override;
	void onMessageTip(QCtmAbstractMessagePtr msg, SWMessageEventType type) override;
private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
