#include "SWApplication.h"
#include "ActivityManager.h"
#include "SWContext.h"
#include "Launcher.h"

#include <QCustomUi/QCustomUi.h>

#include <QLocalSocket>
#include <QLocalServer>

static bool g_qAppInitSucceed{ true };

struct SWApplication::Impl
{

};

SWApplication::SWApplication(int &argc, char **argv, const QString& theme)
    : QCtmApplication(argc, argv, SWApplication::ApplicationFlags)
	, m_impl(std::make_unique<Impl>())
{
    init(theme);
}

SWApplication::~SWApplication()
{
}

void SWApplication::initBeforeStructure()
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Unset);
    QCtmLogManager::initBeforeApp();
}

QCtmMainWindow* SWApplication::mainWindow() const
{
    return SWContext::instance().mainWindow();
}

void SWApplication::setAboutDialog(QCtmDialog* about)
{
	SWContext::instance().mainWindow()->setAboutDialog(about);
}

QCtmDialog* SWApplication::aboutDialog() const
{
	return SWContext::instance().mainWindow()->aboutDialog();
}

SWContextInterface* SWApplication::context() const
{
	return &SWContext::instance();
}

int SWApplication::exec()
{
	if (!g_qAppInitSucceed)
		return -1;
	return QApplication::exec();
}

bool SWApplication::isSuccessful()
{
	return g_qAppInitSucceed;
}

void SWApplication::init(const QString& theme)
{
    QCtmLogManager::instance();
	if (!ActivityManager::instance().loadLibarys())
	{
		g_qAppInitSucceed = false;
		return;
	}

    auto &context = SWContext::instance();
    QString qss = context.defaultStyleSheet(theme);
    setStyleSheet(qss);
	if (!ActivityManager::instance().initActivitys(&context))
	{
		g_qAppInitSucceed = false;
		return;
	}

    context.mainWindow()->loadAlways();

    for (auto act : ActivityManager::instance().activites())
    {
        qss += act->styleSheet(theme);
    }
    setStyleSheet(qss);
}
