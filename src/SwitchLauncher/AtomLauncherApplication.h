#pragma once

#include <QApplication>

class AtomMainWindow;

class AtomLauncherApplication : public QApplication
{
    Q_OBJECT

public:
    /**
     * @brief	theme目前可选择Light和Blue
     */
    AtomLauncherApplication(int &argc, char **argv, const QString& theme = "Light");
    ~AtomLauncherApplication();

    /**
     * @brief		在调用构造函数之前调用该函数进行初始化
     */
    static void initBeforeStructure();

    /**
     * @brief       获取mainWindow地址
     */
    AtomMainWindow* mainWindow() const;
private:
    void init(const QString& theme);
};
