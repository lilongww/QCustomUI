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

#include "QCtmMessageTipButton.h"
#include "QCtmAbstractMessageTipModel.h"
#include "QCtmAbstractMessageTipView.h"
#include "Private/QCtmMessageTipHelper_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QApplication>

struct QCtmMessageTipButton::Impl
{
	QCtmAbstractMessageTipModel* model{ nullptr };
	QCtmAbstractMessageTipView* view{ nullptr };
	QCtmMessageTipHelper* helper{ nullptr };
};

/*!
    \class      QCtmMessageTipButton
    \brief      QCtmMessageTipButton provide a button to show count of the message tip data.
    \inherits   QAbstractButton
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \property   QCtmMessageTipButton::tipColor
    \brief      Holds color for the tip of the button
*/

/*!
    \brief      Constructs a message tip button with \a parent.
*/
QCtmMessageTipButton::QCtmMessageTipButton(QWidget *parent)
	: QAbstractButton(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->helper = new QCtmMessageTipHelper(this);
	connect(this, &QAbstractButton::clicked, this, &QCtmMessageTipButton::onClicked);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}

/*!
    \brief      Destroys the button.
*/
QCtmMessageTipButton::~QCtmMessageTipButton()
{
}

/*!
    \brief      Sets the message tip \a model.
    \sa         model()
*/
void QCtmMessageTipButton::setModel(QCtmAbstractMessageTipModel* model)
{
	if (m_impl->model == model)
		return;
	if (m_impl->model)
		m_impl->model->deleteLater();
	m_impl->model = model;
	connect(m_impl->model, &QAbstractItemModel::rowsRemoved, this, &QCtmMessageTipButton::onModelDataChanged);
	connect(m_impl->model, &QAbstractItemModel::rowsInserted, this, &QCtmMessageTipButton::onModelDataChanged);
	connectView();
	if (this->isVisible())
	{
		update();
	}
}

/*!
    \brief      Returns the message tip model.
    \sa         setModel
*/
QCtmAbstractMessageTipModel* QCtmMessageTipButton::model() const
{
	return m_impl->model;
}

/*!
    \brief      Sets the message tip \a view.
    \sa         view()
*/
void QCtmMessageTipButton::setView(QCtmAbstractMessageTipView* view)
{
	m_impl->view = view;
	connectView();
}

/*!
    \brief      Returns the message tip view.
    \sa         setView
*/
QCtmAbstractMessageTipView* QCtmMessageTipButton::view() const
{
	return m_impl->view;
}

/*!
    \brief      Set the \a color of the tool tip.
    \sa         tipColor()
*/
void QCtmMessageTipButton::setTipColor(const QColor& color)
{
	m_impl->helper->setTipColor(color);
}

/*!
    \brief      Returns the color of the tool tip.
    \sa         setTipColor
*/
const QColor& QCtmMessageTipButton::tipColor() const
{
	return m_impl->helper->tipColor();
}

/*!
    \reimp
*/
void QCtmMessageTipButton::paintEvent([[maybe_unused]] QPaintEvent *event)
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

/*!
    \reimp
*/
QSize QCtmMessageTipButton::sizeHint() const
{
	auto&& size = QAbstractButton::sizeHint();
	size.setWidth(this->iconSize().width() + 20);
	return size.expandedTo(qApp->globalStrut());
}

/*!
    \brief      Sets the model to view.
*/
void QCtmMessageTipButton::connectView()
{
	if (m_impl->view&&m_impl->model)
		m_impl->view->setModel(m_impl->model);
}

/*!
    \brief      Set arguments for the style option, \a opt.
*/
void QCtmMessageTipButton::initStyleOption(QStyleOptionButton* opt)
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

/*!
    \brief      Pop the message tip view when the button is clicked.
*/
void QCtmMessageTipButton::onClicked(bool)
{
	if (this->view())
	{
		if (this->view()->isVisible())
			this->view()->close();
		else
			this->view()->show();
	}
}

/*!
    \brief      Update the button when the message tip model is changed.
*/
void QCtmMessageTipButton::onModelDataChanged()
{
	update();
}

