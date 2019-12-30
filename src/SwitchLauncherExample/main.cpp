#include <QtWidgets/QApplication>

#include <SwitchLauncher/SWApplication.h>
#include <QCustomUi/QCustomUi.h>

int main(int argc, char *argv[])
{
	SWApplication::initBeforeStructure();
	SWApplication a(argc, argv);
	if (a.checkOtherProcess("sw_example"))
		return 0;
	if (a.isSuccessful())
	{
		a.mainWindow()->resize(1000, 600);
		a.mainWindow()->showMaximized();
	}
	return a.exec();
}
