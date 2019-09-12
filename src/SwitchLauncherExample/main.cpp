#include <QtWidgets/QApplication>

#include <SwitchLauncher/SWApplication.h>
#include <QCustomUi/QCustomUi.h>

int main(int argc, char *argv[])
{
	SWApplication::initBeforeStructure();
	SWApplication::setSingleton(true);
	SWApplication::setSingletonKey("sw_example");
	SWApplication a(argc, argv);
	if (a.isSuccessful())
	{
		a.mainWindow()->resize(1000, 600);
		a.mainWindow()->showMaximized();
	}
	return a.exec();
}
