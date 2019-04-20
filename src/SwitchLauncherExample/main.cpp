#include <QtWidgets/QApplication>

#include <SwitchLauncher/SWApplication.h>
#include <QCustomUi/QCustomUi.h>

int main(int argc, char *argv[])
{
	SWApplication::initBeforeStructure();
	SWApplication a(argc, argv);
	a.mainWindow()->resize(1000, 600);
	a.mainWindow()->showMaximized();
	return a.exec();
}
