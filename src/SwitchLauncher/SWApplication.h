#pragma once

#include "switchlauncher_global.h"

#include <QApplication>

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
private:
    void init(const QString& theme);
};
