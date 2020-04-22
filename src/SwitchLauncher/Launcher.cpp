#include "Launcher.h"
#include "ActivityManager.h"
#include "SWContext.h"
#include "SWPluginInterface.h"

#include <QMenuBar>
#include <QToolButton>

struct Launcher::Impl
{
	bool first{ true };
	SWPluginInterface* currentActivity{ nullptr };
	QMenuBar* menuBar{ nullptr };
	QStatusBar* statusBar{ nullptr };

	QStringList themes;
	QCtmMessageTip* msgTip{ nullptr };
	QCtmMessageView* msgView{ nullptr };
	QCtmMessageModel* msgModel{ nullptr };

	QAction* userAction{ nullptr };
	QMenu* help{ nullptr };

	QCtmLogWidget* logWidget{ nullptr };

	QCtmDialog* about{ nullptr };
};

Launcher::Launcher(QWidget *parent)
    : QCtmMainWindow(parent)
    , m_impl(std::make_shared<Impl>())
{
	resize(1000, 600);
    initUi();
}

Launcher::~Launcher()
{
}

void Launcher::initUi()
{
	setNavigationBar(new QCtmNavigationBar(this));
	{//UserAction
		m_impl->userAction = navigationBar()->addUser(QIcon(), "");
		m_impl->userAction->setObjectName("userAction");
		connect(m_impl->userAction, &QAction::triggered, this, [=]() {
			for (auto &&sl : ActivityManager::instance().activitySlots())
			{
				sl->onUserActionTriggered();
			}
		});
	}

	setMessageBar(new QCtmTabWidget);
	m_impl->logWidget = new QCtmLogWidget("SW", this);
	auto tab = messageBar()->addTab(m_impl->logWidget, QObject::tr("LOG"));
    tab->addActions(m_impl->logWidget->actions());

	this->titleWidget()->setShowIcon(false);

	m_impl->statusBar = new QStatusBar(this);
	this->setStatusBar(m_impl->statusBar);
	{
		QToolButton *btn = new QToolButton(m_impl->statusBar);
		btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
		btn->setObjectName("messageBarControlBtn");
		m_impl->statusBar->addPermanentWidget(btn);
		connect(btn, &QToolButton::clicked, this, [=]() {
			if (this->messageBar())
				this->messageBar()->setVisible(!this->messageBar()->isVisible());
		});
	}

	m_impl->menuBar = new QMenuBar(titleWidget());
	connect(m_impl->menuBar, &QMenuBar::triggered, this, [=](QAction* action) {
		for (auto &&actSlot : ActivityManager::instance().activitySlots())
		{
			actSlot->onMenuBarActionTriggered(action);
		}
	});
	this->titleWidget()->setMenuBar(m_impl->menuBar);
#if 0
	{
		auto file = m_impl->menuBar->addMenu(QObject::tr("&File"));

		auto style = file->addMenu(tr("Style"));
		connect(style, &QMenu::aboutToShow, this, [=]() {
			style->clear();
			for (auto &&theme : m_impl->themes)
			{
				auto action = style->addAction(theme);
				action->setData(theme);
				connect(action, &QAction::triggered, this, [=]() {
					QString qss = defaultStyleSheet(theme);
					for (auto act : ActivityManager::instance().activites())
					{
						qss.append(act->styleSheet(theme));
					}
					qApp->setStyleSheet(qss);
				});
			}
		});

		connect(style, &QMenu::triggered, this, [=](QAction* action) {
			auto &&theme = action->data().toString();
			emit themeChanged(theme);
		});

		file->addAction(tr("Preference"));
		file->addSeparator();
		file->addAction(tr("Quit"), [=]() { this->close(); });
	}
#endif

	{
		m_impl->help = m_impl->menuBar->addMenu(tr("&Help"));
		m_impl->help->addAction(tr("Help"));
		auto about = m_impl->help->addAction(tr("About"));
		connect(about, &QAction::triggered, this, [=]() {
			if(m_impl->about)
				m_impl->about->exec();
			});
	}
#if 0
	{
		m_impl->msgTip = new QCtmMessageTip(this);
		m_impl->msgView = new QCtmMessageView(navigationBar());
		m_impl->msgModel = new QCtmMessageModel(this);

		m_impl->msgTip->setModel(m_impl->msgModel);
		m_impl->msgTip->setView(m_impl->msgView);

		QWidgetAction* action = new QWidgetAction(this);
		action->setDefaultWidget(m_impl->msgTip);

		navigationBar()->addAction(action, QCtmNavigationBar::Right);

		connect(m_impl->msgView, &QCtmMessageView::messageClicked, this, [=](QCtmAbstractMessagePtr msg) {
			for (auto&& actSlot : ActivityManager::instance().activitySlots())
			{
				actSlot->onMessageTip(msg, SWMessageEventType::Clicked);
			}
			});

		connect(m_impl->msgView, &QCtmMessageView::closeButtonClicked, this, [=](const QModelIndex& index) {
			for (auto&& actSlot : ActivityManager::instance().activitySlots())
			{
				auto msg = m_impl->msgModel->message(index.row());
				actSlot->onMessageTip(msg, SWMessageEventType::Delete);
			}
			});
	}

	{
		auto help = navigationBar()->addHelp("file:///" + qApp->applicationDirPath() + "/Doc/Help.docx", QIcon());
		help->setObjectName("help");
	}
#endif
}


bool Launcher::setCurrentActivity(SWPluginInterface* activity, const SWUserData& data)
{
    if (m_impl->currentActivity)
    {
        if (!m_impl->currentActivity->currentPluginChangeEvent(false, m_impl->currentActivity, data))
            return false;
		if (activity && !activity->currentPluginChangeEvent(true, m_impl->currentActivity, data))
		{
			m_impl->currentActivity->currentPluginChangeEvent(true, m_impl->currentActivity, data);
			return false;
		}
        takeContentWidget();
        takePropertyBar();
        takeNavigationSideBar();
        takeSubSideBar();
        auto &&actionInfos = m_impl->currentActivity->navigationBarActions();
        for (auto &actionInfo : actionInfos)
        {
            if (!actionInfo.alwaysShow)
            {
                navigationBar()->removeAction(actionInfo.action);
                if (actionInfo.panel != nullptr)
                {
                    actionInfo.panel->close();
                }
            }
        }

		auto &&subMenus = m_impl->currentActivity->subMenus();
		for (auto&& subMenu : subMenus)
		{
			if (!subMenu.alwaysShow)
			{
				this->titleWidget()->menuBar()->removeAction(subMenu.menu->menuAction());
			}
		}

		auto &&tabs = m_impl->currentActivity->messageBarTabPages();
		QWidgetList willDeletePages;
		for (auto &&tab : tabs)
		{
			for (int i = 0;i<messageBar()->count();i++)
			{
				auto page = dynamic_cast<QCtmTabPage*>(messageBar()->widget(i));
				if (page)
				{
					if (page->centralWidget() == tab.widget)
					{
						page->takeCentralWidget();
						willDeletePages.push_back(page);
					}
				}
			}
		}

		for (auto page : willDeletePages)
		{
			delete page;
		}

		for (auto &&wInfo : m_impl->currentActivity->statusBarWidgets())
		{
			for (auto &&obj : this->statusBar()->children())
			{
				if (obj->isWidgetType() && obj == wInfo.widget) 
				{
					this->statusBar()->removeWidget(wInfo.widget);
				}
			}
		}
    }
	else
	{
		if (activity && !activity->currentPluginChangeEvent(true, m_impl->currentActivity, data))
			return false;
	}
    if (!activity)
        return true;
    m_impl->currentActivity = activity;
    if (activity->attributes().testFlag(SWPluginAttribute::SWPA_DontShowPluginNameInTitle) 
        || SWContext::instance().attributes().testFlag(SWContextAttribute::SWCA_DontShowTitle))
        setWindowTitle("");
    else
        setWindowTitle(activity->name());
    if (activity->content())
    {
        setContentWidget(activity->content());
        activity->content()->show();
    }
    if (activity->sideBar())
    {
        setNavigationSideBar(activity->sideBar());
        activity->sideBar()->show();
    }
    if (activity->subSideBar())
    {
        setSubSideBar(activity->subSideBar());
        activity->subSideBar()->show();
    }
    
    auto &&actionInfos = activity->navigationBarActions();
    for (auto &&actionInfo : actionInfos)
    {
		if(actionInfo.alwaysShow)
			continue;
        if (actionInfo.position == QCtmNavigationBar::Right)
        {
            if (!actionInfo.panel)
                navigationBar()->insertAction(0, actionInfo.action, (QCtmNavigationBar::ActionPosition)actionInfo.position);
            else
                navigationBar()->insertPane(0, actionInfo.action, (QCtmNavigationBar::ActionPosition)actionInfo.position, actionInfo.panel);
        }
        else
        {
            if (!actionInfo.panel)
                navigationBar()->addAction(actionInfo.action, (QCtmNavigationBar::ActionPosition)actionInfo.position);
            else
                navigationBar()->addPane(actionInfo.action, (QCtmNavigationBar::ActionPosition)actionInfo.position, actionInfo.panel);
        }
    }

	auto &&subMenus = activity->subMenus();
	for (auto&& subMenu : subMenus)
	{
		if (subMenu.alwaysShow)
			continue;
		this->titleWidget()->menuBar()->insertMenu(m_impl->help->menuAction(), subMenu.menu);
	}

	auto &&tabs = activity->messageBarTabPages();
	for (auto &&tab : tabs)
	{
		auto page = this->messageBar()->addTab(tab.widget, tab.title);
		for (auto &&action : tab.actions)
		{
			page->addAction(action);
		}
	}

	for (auto &&wInfo : activity->statusBarWidgets())
	{
		switch (wInfo.position)
		{
		case StatusBarInfo::Left:
			statusBar()->addWidget(wInfo.widget, wInfo.stretch);
			break;
		case StatusBarInfo::Right:
			statusBar()->insertPermanentWidget(0, wInfo.widget, wInfo.stretch);
			break;
		}
		wInfo.widget->show();
	}

	return true;
}

SWPluginInterface* Launcher::currentActivity() const
{
    return m_impl->currentActivity;
}

void Launcher::setThemes(const QStringList& themes)
{
	m_impl->themes = themes;
}

const QStringList& Launcher::themes() const
{
	return m_impl->themes;
}

void Launcher::addMessageTip(QCtmAbstractMessagePtr msg)
{
	if (!m_impl->msgModel)
		return;
	m_impl->msgModel->addMessage(msg);
	for (auto &&actSlot : ActivityManager::instance().activitySlots())
	{
		actSlot->onMessageTip(msg, SWMessageEventType::Add);
	}
}

void Launcher::setUser(const QString& user)
{
	m_impl->userAction->setText(user);
}

void Launcher::loadAlways()
{
	for (auto &&act : ActivityManager::instance().activites())
	{
		auto &&actionInfos = act->navigationBarActions();
		for (auto &&actionInfo : actionInfos)
		{
			if (actionInfo.alwaysShow)
			{
				navigationBar()->addAction(actionInfo.action, (QCtmNavigationBar::ActionPosition)actionInfo.position);
			}
		}

		auto &&subMenus = act->subMenus();
		for (auto &&subMenu : subMenus)
		{
			if (subMenu.alwaysShow)
			{
				this->titleWidget()->menuBar()->insertMenu(m_impl->help->menuAction(), subMenu.menu);
			}
		}
	}
}

QString Launcher::defaultStyleSheet(const QString& theme) const
{
    if (theme.isEmpty() || theme == "Light")
    {
		QString qss;
		QFile file(":/qss/Resources/Launcher-light.css");
		if (file.open(QFile::ReadOnly))
		{
			qss = file.readAll();
		}
		return std::move(QCtmStyleSheet::defaultStyleSheet() + qss);
    }
    return QString();
}

QCtmLogWidget* Launcher::logWidget() const
{
    return m_impl->logWidget;
}

void Launcher::setAboutDialog(QCtmDialog* about)
{
	m_impl->about = about;
}

QCtmDialog* Launcher::aboutDialog() const
{
	return m_impl->about;
}

void Launcher::showEvent(QShowEvent* e)
{
    if (m_impl->first)
    {
        m_impl->first = false;
        for (auto act : ActivityManager::instance().activites())
        {
			auto action = navigationBar()->addAction(act->icon(), act->name(), QCtmNavigationBar::ActionPosition::Left);
            if (act->attributes().testFlag(SWPluginAttribute::SWPA_DontShowInPluginMenu))
            {
                action->setVisible(false);
            }
			connect(action, &QAction::triggered, this, [=](bool) {setCurrentActivity(act);});
        }
		if (ActivityManager::instance().activites().isEmpty())
			return;
        setCurrentActivity(ActivityManager::instance().activites().first());
        setSubSideBarWidth(250);
    }
	QCtmMainWindow::showEvent(e);
}

void Launcher::closeEvent(QCloseEvent* e)
{
	if (SWContext::instance().attributes().testFlag(SWContextAttribute::SWCA_QuitAlert))
	{
		if (QCtmMessageBox::question(this, QStringLiteral("提示"), QStringLiteral("是否确认退出应用程序？")) != QCtmMessageBox::Yes)
		{
			e->ignore();
			return;
		}
	}
    if (m_impl->currentActivity)
    {
        if (!m_impl->currentActivity->currentPluginChangeEvent(false, m_impl->currentActivity))
        {
            e->ignore();
        }
        else
            setCurrentActivity(nullptr);
    }
}
