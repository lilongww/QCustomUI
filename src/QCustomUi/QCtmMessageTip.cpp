/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/

#include "QCtmMessageTip.h"
#include "QCtmAbstractMessageTipModel.h"
#include "QCtmAbstractMessageTipView.h"
#include "Private/QCtmMessageTipHelper_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>

struct QCtmMessageTip::Impl
{
	QCtmAbstractMessageTipModel* model{ nullptr };
	QCtmAbstractMessageTipView* view{ nullptr };
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
 * @brief		Set the message model.
 */
void QCtmMessageTip::setModel(QCtmAbstractMessageTipModel* model)
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
 * @brief       Get the message model.
 */
QCtmAbstractMessageTipModel* QCtmMessageTip::model() const
{
	return m_impl->model;
}

/**
 * @brief       Set the message view.
 */
void QCtmMessageTip::setView(QCtmAbstractMessageTipView* view)
{
	m_impl->view = view;
	connectView();
}

/**
 * @brief       Get the message view.
 */
QCtmAbstractMessageTipView* QCtmMessageTip::view() const
{
	return m_impl->view;
}

/**
 * @brief       Set the color of the tool tip.
 */
void QCtmMessageTip::setTipColor(const QColor& color)
{
	m_impl->helper->setTipColor(color);
}

/**
 * @brief       Get the color of the tool tip.
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

