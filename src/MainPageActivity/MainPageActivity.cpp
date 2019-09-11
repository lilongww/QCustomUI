#include "MainPageActivity.h"
#include "StyleWidget.h"

#include <QCustomUi/QCustomUi.h>

#include <QLabel>
#include <QTreeWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

struct MainPageActivity::Impl
{
    QIcon icon;
    QWidget *content{ nullptr };
    QCtmTabWidget* messageBar{ nullptr };
    QCtmNavigationSideBar* sideBar{ nullptr };
    QCtmToolBox* subSideBar{ nullptr };
    QList<ActionInfo> actionInfos;
	QList<MenuInfo> menus;
    SWContextInterface* context;
};

MainPageActivity::MainPageActivity()
    : m_impl(std::make_shared<Impl>())
{
}

MainPageActivity::~MainPageActivity()
{

}

int MainPageActivity::id() const
{
    return 0;
}

QIcon MainPageActivity::icon() const
{
    return m_impl->icon;
}

QString MainPageActivity::name() const
{
    return "Home";
}

bool MainPageActivity::initialize(SWContextInterface* context)
{
    //context->setMainWindowShadowless(true);
    m_impl->context = context;
    auto qss = m_impl->context->defaultStyleSheet("light");
    m_impl->content = new StyleWidget;
    m_impl->subSideBar = new QCtmToolBox(nullptr);
    auto projTree = new QTreeWidget;
    projTree->header()->hide();
    projTree->setIndentation(0);
    for (int i = 1 ; i< 4;i++)
    {
        projTree->addTopLevelItem(new QTreeWidgetItem(QStringList()<<QString("Project%1").arg(i)));
    }
    m_impl->subSideBar->addWidget(tr("Test"), projTree);

    {
        auto action = new QAction("Main Page Action",nullptr);
        auto menu = new QCtmNavigationMainMenu;
        menu->addAction("Main Test");
        m_impl->actionInfos.push_back({ action, QCtmNavigationBar::Left, menu});
    }

	auto logo = context->addLogo(QIcon());
	logo->setObjectName("Logo");

	connect(logo, &QAction::triggered, this, [=]() {
		context->setCurrentPlugin(this);
	});

	context->setUserInfo("User", "Admin");

	{
		MenuInfo info;
		info.alwaysShow = false;
		info.menu = new QMenu("TestMenu");
		info.menu->addAction("MainPage");
		info.menu->addAction("Test");
		m_impl->menus.append(info);
	}

    qInfo() << "#AL" << tr("MainPageActivity Inited");
    return true;
}

void MainPageActivity::release()
{
    
}

QString MainPageActivity::styleSheet(const QString& theme) const
{
    return m_impl->context->defaultStyleSheet("Blue");
}

bool MainPageActivity::currentPluginChangeEvent(bool current, const SWPluginInterface* old, const SWUserData& data )
{
    m_impl->context->setMessageBarVisible(true);
	return true;
}

QWidget* MainPageActivity::content() const
{
    return m_impl->content;
}

QCtmNavigationSideBar* MainPageActivity::sideBar() const
{
    return m_impl->sideBar;
}

QWidget* MainPageActivity::subSideBar() const
{
    return m_impl->subSideBar;
}

QList<ActionInfo> MainPageActivity::navigationBarActions() const
{
    return m_impl->actionInfos;
}

QList<TabPageInfo> MainPageActivity::messageBarTabPages() const
{
	return QList<TabPageInfo>();
}

QList<StatusBarInfo> MainPageActivity::statusBarWidgets() const
{
	return {};
}

QList<MenuInfo> MainPageActivity::subMenus() const
{
	return m_impl->menus;
}

void MainPageActivity::onUserActionTriggered()
{
	QMessageBox::information(nullptr, tr("Tips"), tr("User Action Clicked"));
}
