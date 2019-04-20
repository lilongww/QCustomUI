#include "QCtmMaximumSizeButton_p.h"

#include <QStyle>

QCtmMaximumSizeButton::QCtmMaximumSizeButton(QWidget *parent)
	: QPushButton(parent)
	, m_maxsized(false)
{
}

QCtmMaximumSizeButton::~QCtmMaximumSizeButton()
{
}

void QCtmMaximumSizeButton::setMaximumSized(bool max)
{
	this->style()->unpolish(this);
	m_maxsized = max;
	this->style()->polish(this);
}

bool QCtmMaximumSizeButton::maximumSized() const
{
	return m_maxsized;
}
