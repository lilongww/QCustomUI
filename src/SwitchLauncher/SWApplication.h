#pragma once

#include "switchlauncher_global.h"

#include <QApplication>

#include <memory>

class QCtmMainWindow;
class QCtmDialog;

class SWITCHLAUNCHER_EXPORT SWApplication : public QApplication
{
    Q_OBJECT

public:
    /**
     * @brief	theme目前可选择Light和Blue
     */
    SWApplication(int &argc, char **argv, const QString& theme = "Light");
    ~SWApplication();

    /**
     * @brief		在调用构造函数之前调用该函数进行初始化
     */
    static void initBeforeStructure();

    /**
     * @brief       获取mainWindow地址
     */
    QCtmMainWindow* mainWindow() const;

	/**
	 * @brief		设置关于窗口地址	
	 */
	void setAboutDialog(QCtmDialog* about);

	/**
	 * @brief		获取关于窗口地址
	 */
	QCtmDialog* aboutDialog() const;

	/**
	 * @brief		是否初始化成功
	 */
	static bool isSuccessful();

	static int exec();

	/**
	 * @brief		是否以单例模式启动程序，确保该函数在SWApplication构造之前被调用
	 * @param[in]	flag：默认为否
	 */
	static void setSingleton(bool flag);

	/**
	 * @brief		设置单例模式的关键字，设置后另一个持有该关键字的程序无法启动，确保该函数在
		SWApplication构造之前被调用
	 * @param[in]	key：关键字
	 */
	static void setSingletonKey(const QString& key);
private:
    void init(const QString& theme);
	
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
