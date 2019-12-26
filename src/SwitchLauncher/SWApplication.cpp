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
	QLocalServer server;
	QLocalSocket client;
	static QString serverName;
	static bool singleton;
};

bool SWApplication::Impl::singleton{ false };
QString SWApplication::Impl::serverName{"SWApplication"};

SWApplication::SWApplication(int &argc, char **argv, const QString& theme)
    : QApplication(argc, argv)
	, m_impl(std::make_unique<Impl>())
{
    init(theme);
	if (Impl::singleton)
	{
		connect(&m_impl->client
			, qOverload<QLocalSocket::LocalSocketError>(&QLocalSocket::error)
			, this
			, [=]() {
				if (!m_impl->server.listen(Impl::serverName))
				{
					g_qAppInitSucceed = false;
					return;
				}
				connect(&m_impl->server, &QLocalServer::newConnection, this, [this]() {
					auto w = this->focusWidget();
					if (w)
						w->activateWindow();
					});
			});
		connect(&m_impl->client, &QLocalSocket::connected, this, [this]() {
			g_qAppInitSucceed = false;
			});
		m_impl->client.connectToServer(Impl::serverName);
	}
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

int SWApplication::exec()
{
	if (!g_qAppInitSucceed)
		return -1;
	return QApplication::exec();
}

void SWApplication::setSingleton(bool flag)
{
	Impl::singleton = flag;
}

void SWApplication::setSingletonKey(const QString& key)
{
	Impl::serverName = key;
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
