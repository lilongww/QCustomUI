#pragma once

#include "switchlauncher_global.h"

#include <QApplication>

class QCtmMainWindow;

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
private:
    void init(const QString& theme);
};
