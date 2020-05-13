#include "ActivityManager.h"
#include "SWPluginInterface.h"

#include <QCustomUi/QCustomUi.h>

#include <QPluginLoader>
#include <QApplication>
#include <QMessageBox>
#include <QDir>

struct ActivityManager::Impl
{
    QMap<int, SWPluginInterface*> activities;
	QMap<int, SWPluginSlots*> activitySlots;
};

ActivityManager& ActivityManager::instance()
{
    static ActivityManager ins;
    return ins;
}

bool ActivityManager::loadLibarys()
{
    auto actDir = qApp->applicationDirPath() + "/SWPlugins";
    QDir dir(actDir);
#ifdef Q_OS_WIN
    for (auto it : dir.entryList(QStringList() << "*.dll"))
#else
    for (auto it : dir.entryList(QStringList() << "*.so"))
#endif
    {
        QPluginLoader lib(actDir + "/" + it);
        if (lib.load())
        {
            auto p = qobject_cast<SWPluginInterface *>(lib.instance());
            if (p->name().isEmpty())
            {
				QCtmMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Activity name is empty:%1").arg(it));
                return false;
            }
            m_impl->activities.insert(p->id(), p);
			
			auto actSlot = dynamic_cast<SWPluginSlots*>(p);
			if (actSlot)
			{
				m_impl->activitySlots.insert(p->id(), actSlot);
			}
        }
        else
        {
            QCtmMessageBox::warning(nullptr, QObject::tr("Warning"), lib.errorString());
            return false;
        }
    }

    return true;
}

bool ActivityManager::initActivitys(SWContextInterface *context)
{
    for (auto act : m_impl->activities)
    {
        if (!act->initialize(context))
        {
            //QMessageBox::warning(nullptr, QObject::tr("Warning"), act->name() + QObject::tr(" initialize failed"));
            return false;
        }
    }

	for (auto act : m_impl->activities)
	{
		act->allActivitiesInitialized();
	}

    return true;
}

const QMap<int, SWPluginInterface*>& ActivityManager::activites() const
{
    return m_impl->activities;
}

const QMap<int, SWPluginSlots*>& ActivityManager::activitySlots() const
{
	return m_impl->activitySlots;
}

ActivityManager::ActivityManager()
    : m_impl(std::make_shared<Impl>())
{
}

ActivityManager::~ActivityManager()
{
    for (auto act : m_impl->activities)
    {
        act->release();
    }
}
