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
	
	/**
	 * @brief		检测是否有其他进程已启动
	 * @param[in]	key 程序识别码
	 * @return		true 有其他进程已启动
	 */
	bool checkOtherProcess(const QString& key);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
