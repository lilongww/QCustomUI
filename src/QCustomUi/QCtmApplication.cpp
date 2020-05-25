#include "QCtmApplication.h"
#include "QCtmStyleSheet.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QWidget>
#include <QWindow>

struct QCtmApplication::Impl
{
	QLocalServer server;
	QLocalSocket client;
	QString serverName;
};

QCtmApplication::QCtmApplication(int& argc, char** argv, int f/*= ApplicationFlags*/)
	: QApplication(argc, argv, f)
	, m_impl(std::make_unique<Impl>())
{
	setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
}

QCtmApplication::~QCtmApplication()
{
}

bool QCtmApplication::checkOtherProcess(const QString& key)
{
	if (key.isEmpty())
		return false;

	connect(&m_impl->server, &QLocalServer::newConnection, this, [this]() {
		auto ws = this->topLevelWindows();
		if (!ws.isEmpty())
		{
			for (auto w : ws)
			{
				if (w->isVisible())
				{
					w->show();
				}
				else
					w->raise();
			}
		}
		});

	m_impl->client.connectToServer(key);
	if (m_impl->client.waitForConnected(100))
		return true;

	if (!m_impl->server.listen(key))
	{
		return true;
	}
	return false;
}
