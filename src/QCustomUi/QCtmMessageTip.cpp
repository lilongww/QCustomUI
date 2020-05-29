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

/**
 * @brief       设置数据来源
 * @param[in]   model 数据源
 */
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

/**
 * @brief       获取数据源
 * @Return:     数据源
 */
QCtmAbstractMessageModel* QCtmMessageTip::model() const
{
	return m_impl->model;
}

/**
 * @brief       设置消息显示视图
 * @param[in] 	view 视图地址
 */
void QCtmMessageTip::setView(QCtmAbstractMessageView* view)
{
	m_impl->view = view;
	connectView();
}

/**
 * @brief       获取消息显示视图
 * @Return:   	视图地址
 */
QCtmAbstractMessageView* QCtmMessageTip::view() const
{
	return m_impl->view;
}

/**
 * @brief       设置提示背景色
 * @param[in]   color 背景色
 */
void QCtmMessageTip::setTipColor(const QColor& color)
{
	m_impl->helper->setTipColor(color);
}

/**
 * @brief       获取提示背景色
 * @Return:   	背景色
 */
const QColor& QCtmMessageTip::tipColor() const
{
	return m_impl->helper->tipColor();
}

void QCtmMessageTip::paintEvent([[maybe_unused]] QPaintEvent *event)
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
	auto&& size = QAbstractButton::sizeHint();
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

