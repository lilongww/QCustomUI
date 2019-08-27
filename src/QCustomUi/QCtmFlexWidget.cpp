#include "QCtmFlexWidget.h"

struct QCtmFlexWidget::Impl
{
	Qt::Orientation ori{ Qt::Horizontal };
};

QCtmFlexWidget::QCtmFlexWidget(QWidget *parent)
	: QCtmAbstractDockPanel(parent)
	, m_impl(std::make_unique<Impl>())
{

}

QCtmFlexWidget::QCtmFlexWidget(Qt::Orientation orientation, QWidget* parent /*= nullptr*/)
	: QCtmFlexWidget(parent)
{
	m_impl->ori = orientation;
}

QCtmFlexWidget::~QCtmFlexWidget()
{
}

void QCtmFlexWidget::setOrientation(Qt::Orientation orientation)
{
	m_impl->ori = orientation;
}

Qt::Orientation QCtmFlexWidget::orientation() const
{
	return m_impl->ori;
}

void QCtmFlexWidget::addDockPanel(QCtmAbstractDockPanel* panel)
{

}

void QCtmFlexWidget::removeDockPanel(QCtmAbstractDockPanel* panel)
{

}

void QCtmFlexWidget::insertDockPanel(int index, QCtmAbstractDockPanel* panel)
{

}

int QCtmFlexWidget::indexOf(QCtmAbstractDockPanel* panel) const
{
	//TODO:
	return 0;
}

QCtmAbstractDockPanel* QCtmFlexWidget::panel() const
{
	//TODO:
	return nullptr;
}
