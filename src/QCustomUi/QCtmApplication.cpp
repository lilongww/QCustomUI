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

/**
 * @brief		检测是否有其他进程已启动
 * @param[in]	key 程序识别码
 * @return		true 有其他进程已启动
 */
bool QCtmApplication::checkOtherProcess(const QString& key)
{
	if (key.isEmpty())
		return false;

	connect(&m_impl->server, &QLocalServer::newConnection, this, [this]() {
		const auto& ws = this->topLevelWindows();
		if (!ws.isEmpty())
		{
			for (const auto& w : ws)
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
