#include "SWContext.h"
#include "Launcher.h"
#include "ActivityManager.h"

#include <QFile>
#include <QMap>
#include <QList>
#include <QApplication>
#include <QTranslator>

struct SWContext::Impl
{
	Launcher* launcher;
    QMap<QString, QList<QTranslator*>> translators;
    SWContextAttributes attributes;
	QString currentLanguage{ "CN" };

    void initTranslators(QObject* parent)
    {
        {//CN
            QTranslator* translator = new QTranslator(parent);
            if (translator->load(":/QCustomUi/qcustomui_zh.qm1"), qApp->applicationDirPath().toLocal8Bit().data());
                translators["CN"].push_back(translator);

            translator = new QTranslator(parent);
            if (translator->load(":/translation/switchlauncher_zh.qm"), qApp->applicationDirPath().toLocal8Bit().data());
                translators["CN"].push_back(translator);
        }
    }
};

SWContext::SWContext()
	: m_impl(std::make_shared<Impl>())
{
    m_impl->initTranslators(qApp);
    this->setLanguage("CN");
	m_impl->launcher = new Launcher;
}


SWContext::~SWContext()
{
}

SWContext& SWContext::instance()
{
    static SWContext ins;
    return ins;
}

Launcher* SWContext::mainWindow() const
{
	return m_impl->launcher;
}

SWPluginInterface* SWContext::plugin(const QString& name) const
{
	auto &activities = ActivityManager::instance().activites();
	for (auto activity : activities)
	{
		if (activity->name() == name)
		{
			return activity;
		}
	}
	return nullptr;
}

QList<SWPluginInterface*> SWContext::plugins() const
{
	auto &activities = ActivityManager::instance().activites();
	return std::move(activities.values());
}

QStringList SWContext::activitiesNames() const
{
	auto &activities = ActivityManager::instance().activites();
	QStringList names;
	for (auto activity : activities)
	{
		names.push_back(activity->name());
	}
	return std::move(names);
}

bool SWContext::setCurrentPlugin(SWPluginInterface* activity, const SWUserData &data)
{
	return m_impl->launcher->setCurrentActivity(activity, data);
}

void SWContext::setThemeStyles(const QStringList& themes)
{
	m_impl->launcher->setThemes(themes);
}

const QStringList& SWContext::themeStyles() const
{
	return m_impl->launcher->themes();
}

void SWContext::addMessageTip(QCtmAbstractMessagePtr msg)
{
	m_impl->launcher->addMessageTip(msg);
}

QAction* SWContext::addLogo(const QIcon& icon, ActionPosition pos /*= 0*/)
{
    m_impl->launcher->navigationBar()->insertSeparator(0, (QCtmNavigationBar::ActionPosition)pos);
	return m_impl->launcher->navigationBar()->insertLogo(0, icon, (QCtmNavigationBar::ActionPosition)pos);
}

void SWContext::setUserInfo(const QString& user, const QString& authority)
{
    if (authority.isEmpty())
        m_impl->launcher->setUser(user);
    else
	    m_impl->launcher->setUser(user + "\n(" + authority + ")");
}

void SWContext::setMessageBarVisible(bool visible)
{
    if (m_impl->launcher->messageBar())
    {
        m_impl->launcher->messageBar()->setVisible(visible);
    }
}

bool SWContext::messageBarVisible() const
{
    if (m_impl->launcher->messageBar())
    {
        return m_impl->launcher->messageBar()->isVisible();
    }
    return false;
}

SWPluginInterface* SWContext::currentPlugin() const
{
    return m_impl->launcher->currentActivity();
}

void SWContext::setLanguage(const QString& language)
{
    auto oldTranslators = m_impl->translators.value(m_impl->currentLanguage);
    auto newTranslators = m_impl->translators.value(language);
	m_impl->currentLanguage = language;
    for (auto oldTranslator : oldTranslators)
    {
        qApp->removeTranslator(oldTranslator);
    }

    for (auto newTranslator : newTranslators)
    {
        qApp->installTranslator(newTranslator);
    }
}

const QString& SWContext::language() const
{
    return m_impl->currentLanguage;
}

void SWContext::setAttributes(SWContextAttributes attributes)
{
    m_impl->attributes = attributes;
    if (attributes.testFlag(SWContextAttribute::SWCA_LogInsertDESC))
    {
        m_impl->launcher->logWidget()->setLogInsertMode(DESC);
    }
    else
        m_impl->launcher->logWidget()->setLogInsertMode(ASC);
}

void SWContext::setAttribute(SWContextAttribute attribute, bool enable)
{
    m_impl->attributes.setFlag(attribute, enable);
}

SWContextAttributes SWContext::attributes() const
{
    return m_impl->attributes;
}

void SWContext::setMessageBarCurrentIndex(int index)
{
	m_impl->launcher->messageBar()->setCurrentIndex(index);
}

int SWContext::messageBarCurrentIndex() const
{
	return m_impl->launcher->messageBar()->currentIndex();
}

QString SWContext::defaultStyleSheet(const QString& theme) const
{
    return m_impl->launcher->defaultStyleSheet(theme);
}
