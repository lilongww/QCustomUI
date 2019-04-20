#include "EditActivity.h"
#include "ProjectTree.h"
#include "OperationTree.h"

#include <QCustomUi/QCustomUi.h>

#include <QLabel>
#include <QTreeWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

struct EditActivity::Impl
{
    QIcon icon;
    QLabel *content{ nullptr };
    QCtmTabWidget* messageBar{ nullptr };
    QCtmNavigationSideBar* sideBar{ nullptr };
    QCtmToolBox* subSideBar{ nullptr };
    QList<ActionInfo> actionInfos;
};

EditActivity::EditActivity()
    : m_impl(std::make_shared<Impl>())
{
}

EditActivity::~EditActivity()
{

}

int EditActivity::id() const
{
    return 1;
}

QIcon EditActivity::icon() const
{
    return m_impl->icon;
}

QString EditActivity::name() const
{
    return "Edit";
}

bool EditActivity::initialize(SWContextInterface* context)
{
    m_impl->content = new QLabel(tr("EditActivity Content"));
    m_impl->messageBar = new QCtmTabWidget;
    auto log = new QCtmLogWidget("EditLog", nullptr);
    m_impl->messageBar->addTab(log, tr("LOG"));
    m_impl->subSideBar = new QCtmToolBox(nullptr);
    auto tree = new QCtmTree;
    m_impl->subSideBar->addWidget(tr("EXPLORER"), tree);
    tree->addWidget(tr("Auto Command"), new ProjectTree);
    tree->addWidget(tr("Manual Command"), new QWidget);
    tree->addWidget(tr("Telemetry"), new QWidget);
    m_impl->subSideBar->addWidget(tr("OPERATION"), new OperationTree);
    tree->setAllExpand(true);

    {
        auto action = new QWidgetAction(nullptr);
        auto editor = new QLineEdit(nullptr);
        action->setDefaultWidget(editor);
        m_impl->actionInfos.push_back({ action, QCtmNavigationBar::Right });

        auto a = new QAction("Edit Test",nullptr);
        m_impl->actionInfos.push_back({ a, QCtmNavigationBar::Right });
    }

    qInfo() << "#AL" << tr("EditActivity Inited");
    return true;
}

void EditActivity::release()
{

}

QString EditActivity::styleSheet(const QString& style) const
{
    return "";
}

bool EditActivity::currentPluginChangeEvent(bool current, const SWPluginInterface* old, const SWUserData& data)
{
	return true;
}

QWidget* EditActivity::content() const
{
    return m_impl->content;
}

QCtmNavigationSideBar* EditActivity::sideBar() const
{
    return m_impl->sideBar;
}

QWidget* EditActivity::subSideBar() const
{
    return m_impl->subSideBar;
}

QList<ActionInfo> EditActivity::navigationBarActions() const
{
    return m_impl->actionInfos;
}

QList<TabPageInfo> EditActivity::messageBarTabPages() const
{
	return {};
}

QList<StatusBarInfo> EditActivity::statusBarWidgets() const
{
	return {};
}

QList<MenuInfo> EditActivity::subMenus() const
{
	return {};
}

void EditActivity::onMenuBarActionTriggered(QAction* action)
{
	QCtmMessageBox::information(nullptr, tr("Tips"), action->text());
}

void EditActivity::onMessageTip(QCtmAbstractMessagePtr msg, SWMessageEventType type)
{

}
