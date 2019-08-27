#include "QCtmDockHelper.h"
#include "QCtmFlexWidget.h"

QCtmDockHelper::QCtmDockHelper(QCtmFlexWidget*parent)
	: QWidget(parent)
{
	init();
}

QCtmDockHelper::~QCtmDockHelper()
{
}

void QCtmDockHelper::init()
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
}
