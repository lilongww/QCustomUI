/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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
#include "Private/QCtmMessageTipHelper_p.h"
#include "QCtmAbstractMessageTipModel.h"
#include "QCtmAbstractMessageTipView.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOption>

struct QCtmMessageTipButton::Impl
{
    QCtmAbstractMessageTipModel* model { nullptr };
    QCtmAbstractMessageTipView* view { nullptr };
    QCtmMessageTipHelper* helper { nullptr };
};

/*!
    \class      QCtmMessageTipButton
    \brief      消息提示按钮.
    \inherits   QAbstractButton
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmMessageTipButton.h
*/

/*!
    \property   QCtmMessageTipButton::tipColor
    \brief      消息提示数量的颜色.
*/

/*!
    \brief      构造一个父对象为 \a parent 的消息提示按钮.
*/
QCtmMessageTipButton::QCtmMessageTipButton(QWidget* parent) : QAbstractButton(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->helper = new QCtmMessageTipHelper(this);
    connect(this, &QAbstractButton::clicked, this, &QCtmMessageTipButton::onClicked);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFocusPolicy(Qt::NoFocus);
    setIcon(QIcon(":/QCustomUi/Resources/messageTip-light.png"));
    setTipColor(0xF54545);
    auto pal = this->palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);
}

/*!
    \brief      销毁该消息提示按钮对象.
*/
QCtmMessageTipButton::~QCtmMessageTipButton() {}

/*!
    \brief      设置消息提示按钮的数据来源 \a model.
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
    connect(m_impl->model, &QAbstractItemModel::modelReset, this, &QCtmMessageTipButton::onModelDataChanged);
    connectView();
    if (this->isVisible())
    {
        update();
    }
}

/*!
    \brief      返回消息提示按钮的数据来源 model.
    \sa         setModel
*/
QCtmAbstractMessageTipModel* QCtmMessageTipButton::model() const { return m_impl->model; }

/*!
    \brief      设置消息提示按钮的拓展列表视图 \a view.
    \sa         view()
*/
void QCtmMessageTipButton::setView(QCtmAbstractMessageTipView* view)
{
    m_impl->view = view;
    connectView();
}

/*!
    \brief      返回消息提示按钮的拓展列表视图.
    \sa         setView
*/
QCtmAbstractMessageTipView* QCtmMessageTipButton::view() const { return m_impl->view; }

/*!
    \brief      设置消息提示数量的颜色 \a color.
    \sa         tipColor()
*/
void QCtmMessageTipButton::setTipColor(const QColor& color) { m_impl->helper->setTipColor(color); }

/*!
    \brief      返回消息提示数量的颜色.
    \sa         setTipColor
*/
const QColor& QCtmMessageTipButton::tipColor() const { return m_impl->helper->tipColor(); }

/*!
    \reimp
*/
void QCtmMessageTipButton::paintEvent([[maybe_unused]] QPaintEvent* event)
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
    return size;
}

/*!
    \brief      连接 model 和视图.
*/
void QCtmMessageTipButton::connectView()
{
    if (m_impl->view && m_impl->model)
        m_impl->view->setModel(m_impl->model);
}

/*!
    \brief      初始化样式选项 \a opt.
*/
void QCtmMessageTipButton::initStyleOption(QStyleOptionButton* opt)
{
    opt->icon     = this->icon();
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
    \brief      响应点击操作.
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
    \brief      消息变化时更新界面.
*/
void QCtmMessageTipButton::onModelDataChanged() { update(); }
