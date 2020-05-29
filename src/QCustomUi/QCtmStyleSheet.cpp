#include "QCtmStyleSheet.h"

#include <QFile>

QString QCtmStyleSheet::defaultStyleSheet()
{
	QString qss;
	const QStringList qssFiles{ {":/stylesheet/Resources/stylesheet/default-light.css"}
		,{":/stylesheet/Resources/stylesheet/sw-light.css"} };
	for (const auto &qssFile : qssFiles)
	{
		QFile file(qssFile);
		if (file.open(QFile::ReadOnly))
		{
			qss.append(file.readAll());
		}
	}
	return qss;
}
