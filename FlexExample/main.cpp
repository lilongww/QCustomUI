#include "FlexExample.h"
#include <QtWidgets/QApplication>

#include <QCustomUi/QCtmStyleSheet.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyleSheet(QCtmStyleSheet::defaultStyleSheet());
	FlexExample w;
	w.show();
	return a.exec();
}
