#pragma once

#include "qcustomui_global.h"

#include <QApplication>

#include <memory>

class QCUSTOMUI_EXPORT QCtmApplication : public QApplication
{
	Q_OBJECT

public:
	QCtmApplication(int& argc, char** argv, int = ApplicationFlags);
	~QCtmApplication();
	
	bool checkOtherProcess(const QString& key);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
