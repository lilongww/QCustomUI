#include "SWApplication.h"
#include "ActivityManager.h"
#include "SWContext.h"
#include "Launcher.h"

#include <QCustomUi/QCustomUi.h>

SWApplication::SWApplication(int &argc, char **argv, const QString& theme)
    : QApplication(argc, argv)
{
    init(theme);
}

SWApplication::~SWApplication()
{
}

void SWApplication::initBeforeStructure()
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCtmLogManager::initBeforeApp();
}

QCtmMainWindow* SWApplication::mainWindow() const
{
    return SWContext::instance().mainWindow();
}

void SWApplication::init(const QString& theme)
{
    QCtmLogManager::instance();
    if (!ActivityManager::instance().loadLibarys())
        return;

    auto &context = SWContext::instance();
    QString qss = context.defaultStyleSheet(theme);
    setStyleSheet(qss);
    if (!ActivityManager::instance().initActivitys(&context))
        return;

    context.mainWindow()->loadAlways();

    //auto themeName = context.themeStyles().isEmpty() ? QString() : context.themeStyles().front();
    for (auto act : ActivityManager::instance().activites())
    {
        qss += act->styleSheet(theme);
    }
    setStyleSheet(qss);
}
