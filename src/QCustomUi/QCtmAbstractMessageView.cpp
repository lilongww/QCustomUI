#include "QCtmAbstractMessageView.h"
#include "QCtmAbstractMessageModel.h"

#include <QEvent>


QCtmAbstractMessageView::QCtmAbstractMessageView(QCtmNavigationBar *parent)
	: QCtmNavigationSidePanel(parent)
{
	this->setDockArea(QCtmNavigationSidePanel::DockArea::Right);
}

QCtmAbstractMessageView::~QCtmAbstractMessageView()
{
}

