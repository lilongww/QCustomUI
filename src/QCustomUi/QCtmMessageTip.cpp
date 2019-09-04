#include "QCtmMessageTip.h"
#include "QCtmAbstractMessageModel.h"
#include "QCtmAbstractMessageView.h"
#include "Private/QCtmMessageTipHelper_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>

struct QCtmMessageTip::Impl
{
	QCtmAbstractMessageModel* model{ nullptr };
	QCtmAbstractMessageView* view{ nullptr };

	QCtmMessageTipHelper* helper{ nullptr };
};

QCtmMessageTip::QCtmMessageTip(QWidget *parent)
	: QAbstractButton(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->helper = new QCtmMessageTipHelper(this);
	connect(this, &QAbstractButton::clicked, this, &QCtmMessageTip::onClicked);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}

QCtmMessageTip::~QCtmMessageTip()
{
}

void QCtmMessageTip::setModel(QCtmAbstractMessageModel* model)
{
	if (m_impl->model == model)
		return;
	if (m_impl->model)
		m_impl->model->deleteLater();
	m_impl->model = model;
	connect(m_impl->model, &QAbstractItemModel::rowsRemoved, this, &QCtmMessageTip::onModelDataChanged);
	connect(m_impl->model, &QAbstractItemModel::rowsInserted, this, &QCtmMessageTip::onModelDataChanged);
	connectView();
	if (this->isVisible())
	{
		update();
	}
}

QCtmAbstractMessageModel* QCtmMessageTip::model() const
{
	return m_impl->model;
}

void QCtmMessageTip::setView(QCtmAbstractMessageView* view)
{
	m_impl->view = view;
	connectView();
}

QCtmAbstractMessageView* QCtmMessageTip::view() const
{
	return m_impl->view;
}

void QCtmMessageTip::setTipColor(QColor color)
{
	m_impl->helper->setTipColor(color);
}

QColor QCtmMessageTip::tipColor() const
{
	return m_impl->helper->tipColor();
}

void QCtmMessageTip::paintEvent(QPaintEvent *event)
{
	QStyleOptionButton opt;
	opt.initFrom(this);
	initStyleOption(&opt);

	QPainter p(this);

	this->style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);

	if (m_impl->model)
	{
		m_impl->helper->drawMessageTip(m_impl->model->rowCount(), p, this);
	}
}

QSize QCtmMessageTip::sizeHint() const
{
	auto size = QAbstractButton::sizeHint();
	size.setWidth(this->iconSize().width() + 20);
	return size.expandedTo(qApp->globalStrut());
}

void QCtmMessageTip::connectView()
{
	if (m_impl->view&&m_impl->model)
		m_impl->view->setModel(m_impl->model);
}

void QCtmMessageTip::initStyleOption(QStyleOptionButton* opt)
{
	opt->icon = this->icon();
	opt->iconSize = this->iconSize();
	opt->features = QStyleOptionButton::DefaultButton;

	if (this->isDown())
	{
		opt->state = QStyle::State_Raised | QStyle::State_Sunken;
	}
	if (this->isChecked())
		opt->state = QStyle::State_On;
}

void QCtmMessageTip::onClicked(bool)
{
	if (this->view())
	{
		if (this->view()->isVisible())
			this->view()->close();
		else
			this->view()->show();
	}
}

void QCtmMessageTip::onModelDataChanged()
{
	update();
}

