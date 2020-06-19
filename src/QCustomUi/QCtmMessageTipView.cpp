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

#include "QCtmMessageTipView.h"
#include "QCtmAbstractMessageTipModel.h"
#include "Private/QCtmMessageViewDelegate_p.h"

#include <QListView>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QHelpEvent>
#include <QEvent>
#include <QToolTip>

struct QCtmMessageTipView::Impl
{
    QListView* view{ nullptr };
    QCtmAbstractMessageTipModel* model{ nullptr };
    QCtmMessageViewDelegate* delegate{ nullptr };

    QColor titleColor;
    QColor timeColor;
};

/*!
    \class      QCtmMessageTipView
    \brief      QCtmMessageTipView provide a common message tip view to show the message tip datas.
    \inherits   QCtmAbstractMessageTipView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \property   QCtmMessageTipView::decoration
    \brief      Holds the decoration color of the message tip.
*/

/*!
    \property   QCtmMessageTipView::titleColor
    \brief      Holds the title color of the message tip.
*/

/*!
    \property   QCtmMessageTipView::timeColor
    \brief      Holds the time color of the message tip.
*/

/*!
    \property   QCtmMessageTipView::closeButtonIcon
    \brief      Holds the icon of close button of the message tip.
*/

/*!
    \fn         void QCtmMessageTipView::closeButtonClicked(const QModelIndex& index)
    \brief      Emit this signal when the close button of message tip with \a index has been clicked.
    \sa         messageClicked
*/

/*!
    \fn         void QCtmMessageTipView::messageClicked(QCtmAbstractMessageTipDataPtr message);
    \brief      Emit this signal when the title of message tip has been clicked, and given the \a message pointer.
    \sa         closeButtonClicked
*/

/*!
    \brief      Constructs a message tip view with \a parent.
*/
QCtmMessageTipView::QCtmMessageTipView(QCtmNavigationBar* parent)
    : QCtmAbstractMessageTipView(parent)
    , m_impl(std::make_unique<Impl>())
{
    this->setTitleBarVisible(true);
    setPopup(false);
    m_impl->view = new QListView(this);
    m_impl->view->setMouseTracking(true);

    m_impl->delegate = new QCtmMessageViewDelegate(m_impl->view);
    m_impl->view->setItemDelegate(m_impl->delegate);
    setWidget(m_impl->view);
    setFixedWidth(300);

    connect(m_impl->delegate, &QCtmMessageViewDelegate::closeButtonClicked, this, &QCtmMessageTipView::closeButtonClicked);
    connect(m_impl->delegate, &QCtmMessageViewDelegate::closeButtonClicked, this, &QCtmMessageTipView::onCloseButtonClicked);
    connect(m_impl->delegate, &QCtmMessageViewDelegate::titleClicked, this, &QCtmMessageTipView::onTitleClicked);
    m_impl->view->verticalScrollBar()->installEventFilter(this);

    qRegisterMetaType<QCtmAbstractMessageTipDataPtr>("QCtmAbstractMessagePtr");
}

/*!
    \brief      Destroys the message tip view.
*/
QCtmMessageTipView::~QCtmMessageTipView()
{
}

/*!
    \brief      Sets the given \a model.
    \sa         model()
*/
void QCtmMessageTipView::setModel(QCtmAbstractMessageTipModel* model)
{
    m_impl->model = model;
    setTimeColor(m_impl->timeColor);
    setTitleColor(m_impl->titleColor);
    m_impl->view->setModel(model);
}

/*!
    \brief      Returns the model.
    \sa         setModel
*/
QCtmAbstractMessageTipModel* QCtmMessageTipView::model() const
{
    return m_impl->model;
}

/*!
    \brief      Set the \a color of the decoration.
    \sa         decoration()
*/
void QCtmMessageTipView::setDecoration(const QColor& color)
{
    m_impl->delegate->setDecoration(color);
}

/*!
    \brief      Returns the color of the decoration.
    \sa         setDecoration
*/
const QColor& QCtmMessageTipView::decoration() const
{
    return m_impl->delegate->decoration();
}

/*!
    \brief      Set the \a color of the title.
    \sa         titleColor()
*/
void QCtmMessageTipView::setTitleColor(const QColor& color)
{
    m_impl->titleColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("titlecolor", color);
}

/*!
    \brief      Returns the color of the title.
    \sa         setTitleColor
*/
const QColor& QCtmMessageTipView::titleColor() const
{
    return m_impl->titleColor;
}

/*!
    \brief      Set the \a color of the time.
    \sa         timeColor()
*/
void QCtmMessageTipView::setTimeColor(const QColor& color)
{
    m_impl->timeColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("timecolor", color);
}

/*!
    \brief      Returns the color of the time.
    \sa         setTimeColor
*/
const QColor& QCtmMessageTipView::timeColor() const
{
    return m_impl->timeColor;
}

/*!
    \brief      Set the \a icon of the close button.
    \sa         closeButtonIcon()
*/
void QCtmMessageTipView::setCloseButtonIcon(const QPixmap& icon)
{
    m_impl->delegate->setCloseButtonIcon(icon);
}

/*!
    \brief      Returns the icon of the close button.
    \sa         setCloseButtonIcon
*/
const QPixmap& QCtmMessageTipView::closeButtonIcon() const
{
    return m_impl->delegate->closeButtonIcon();
}

/*!
    \reimp
*/
void QCtmMessageTipView::resizeEvent(QResizeEvent* e)
{
    QCtmAbstractMessageTipView::resizeEvent(e);
    m_impl->view->reset();
}

/*!
    \reimp
*/
void QCtmMessageTipView::showEvent(QShowEvent* e)
{
    QCtmAbstractMessageTipView::showEvent(e);
}

/*!
    \reimp
*/
bool QCtmMessageTipView::eventFilter(QObject* o, QEvent* e)
{
    if (o == m_impl->view->verticalScrollBar())
    {
        if (e->type() == QEvent::Show || e->type() == QEvent::Hide)
            m_impl->view->reset();
    }
    return QCtmNavigationSidePane::eventFilter(o, e);
}

/*!
    \brief      Remove the message tip when the close button clicked.
*/
void QCtmMessageTipView::onCloseButtonClicked(const QModelIndex& index)
{
    if (m_impl->model)
    {
        m_impl->model->removeMessage(m_impl->model->message(index.row()));
        m_impl->view->viewport()->update();
    }
}

/*!
    \brief      Emit messageClicked when the message tip title clicked.
*/
void QCtmMessageTipView::onTitleClicked(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto msg = m_impl->model->message(index.row());
        emit messageClicked(msg);
    }
}
