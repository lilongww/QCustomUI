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

#include "QCtmMessageTipView.h"
#include "Private/QCtmMessageViewDelegate_p.h"
#include "QCtmAbstractMessageTipModel.h"
#include "QCtmMessageTipData.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QHelpEvent>
#include <QListView>
#include <QScrollBar>
#include <QToolButton>
#include <QToolTip>

struct QCtmMessageTipView::Impl
{
    QListView* view { nullptr };
    QCtmAbstractMessageTipModel* model { nullptr };
    QCtmMessageViewDelegate* delegate { nullptr };
    QToolButton* clearAll;

    QColor titleColor;
    QColor timeColor;
};

/*!
    \class      QCtmMessageTipView
    \brief      QCtmMessageTipView 是显示具体消息的列表.
    \inherits   QCtmAbstractMessageTipView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmMessageTipView.h
*/

/*!
    \property   QCtmMessageTipView::decoration
    \brief      装饰条的颜色.
*/

/*!
    \property   QCtmMessageTipView::titleColor
    \brief      消息标题的颜色.
*/

/*!
    \property   QCtmMessageTipView::timeColor
    \brief      时间的颜色.
*/

/*!
    \property   QCtmMessageTipView::closeButtonIcon
    \brief      关闭按钮的图标.
*/

/*!
    \fn         void QCtmMessageTipView::closeButtonClicked(const QModelIndex& index)
    \brief      当  \a index 位置的消息的关闭按钮被点击时，发送该信号.
    \sa         messageClicked
*/

/*!
    \fn         void QCtmMessageTipView::messageClicked(QCtmMessageTipDataPtr message);
    \brief      当消息被点击时，发送该信号 \a message.
    \sa         closeButtonClicked
*/

/*!
    \fn         bool QCtmMessageTipView::aboutToCloseMessage(QCtmMessageTipDataPtr message);
    \brief      如果要在关闭消息时，弹出确认窗口，连接该信号，并在槽函数中返回是否删除 \a message.
*/

/*!
    \fn         bool QCtmMessageTipView::aboutToClearAllMessages();
    \brief      如果要在清除所有消息时，弹出确认窗口，连接该信号，并在槽函数中返回是否删除.
*/

/*!
    \brief      构造一个父对象为 \a parent 的消息列表.
*/
QCtmMessageTipView::QCtmMessageTipView(QCtmNavigationBar* parent) : QCtmAbstractMessageTipView(parent), m_impl(std::make_unique<Impl>())
{
    this->setTitleBarVisible(true);
    setPopup(false);
    m_impl->clearAll = new QToolButton(this);
    m_impl->clearAll->setObjectName("qcustomui_clearAll");
    m_impl->clearAll->setCursor(Qt::PointingHandCursor);
    m_impl->clearAll->setText(tr("Clear All"));
    m_impl->view = new QListView(this);
    m_impl->view->setObjectName("qcustomui_tipview");
    m_impl->view->setMouseTracking(true);

    auto base = new QWidget(this);
    {
        auto layout = new QVBoxLayout(base);
        layout->setContentsMargins(0, 0, 0, 0);
        {
            auto btnBase = new QWidget(this);
            btnBase->setObjectName("qcustomui_clearAllBase");
            auto l = new QHBoxLayout(btnBase);
            l->addStretch(1);
            l->addWidget(m_impl->clearAll);
            l->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(btnBase);
        }
        layout->addWidget(m_impl->view);
        layout->setSpacing(0);
        setWidget(base);
    }

    m_impl->delegate = new QCtmMessageViewDelegate(m_impl->view);
    m_impl->view->setItemDelegate(m_impl->delegate);
    setFixedWidth(300);

    connect(m_impl->delegate, &QCtmMessageViewDelegate::closeButtonClicked, this, &QCtmMessageTipView::closeButtonClicked);
    connect(m_impl->delegate, &QCtmMessageViewDelegate::closeButtonClicked, this, &QCtmMessageTipView::onCloseButtonClicked);
    connect(m_impl->delegate, &QCtmMessageViewDelegate::titleClicked, this, &QCtmMessageTipView::onTitleClicked);
    connect(m_impl->clearAll, &QToolButton::clicked, this, &QCtmMessageTipView::clearAll);
    connect(this, &QCtmMessageTipView::aboutToCloseMessage, this, [] { return true; });
    connect(this, &QCtmMessageTipView::aboutToClearAllMessages, this, [] { return true; });
    m_impl->view->verticalScrollBar()->installEventFilter(this);

    qRegisterMetaType<QCtmAbstractMessageTipDataPtr>("QCtmAbstractMessagePtr");
    setDecoration(0xA7A7A7);
    setCloseButtonIcon(QPixmap(":/QCustomUi/Resources/close-light.png"));
    setTitleColor(0x303030);
    setTimeColor(0x656464);
}

/*!
    \brief      销毁该消息列表.
*/
QCtmMessageTipView::~QCtmMessageTipView() {}

/*!
    \brief      设置消息 \a model.
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
    \brief      返回消息 model.
    \sa         setModel
*/
QCtmAbstractMessageTipModel* QCtmMessageTipView::model() const { return m_impl->model; }

/*!
    \brief      设置装饰颜色 \a color.
    \sa         decoration()
*/
void QCtmMessageTipView::setDecoration(const QColor& color) { m_impl->delegate->setDecoration(color); }

/*!
    \brief      返回装饰颜色.
    \sa         setDecoration
*/
const QColor& QCtmMessageTipView::decoration() const { return m_impl->delegate->decoration(); }

/*!
    \brief      设置标题栏颜色 \a color.
    \sa         titleColor()
*/
void QCtmMessageTipView::setTitleColor(const QColor& color)
{
    m_impl->titleColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("titlecolor", color);
}

/*!
    \brief      返回标题栏颜色.
    \sa         setTitleColor
*/
const QColor& QCtmMessageTipView::titleColor() const { return m_impl->titleColor; }

/*!
    \brief      设置时间颜色 \a color.
    \sa         timeColor()
*/
void QCtmMessageTipView::setTimeColor(const QColor& color)
{
    m_impl->timeColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("timecolor", color);
}

/*!
    \brief      返回时间颜色.
    \sa         setTimeColor
*/
const QColor& QCtmMessageTipView::timeColor() const { return m_impl->timeColor; }

/*!
    \brief      设置关闭按钮图标 \a icon.
    \sa         closeButtonIcon()
*/
void QCtmMessageTipView::setCloseButtonIcon(const QPixmap& icon) { m_impl->delegate->setCloseButtonIcon(icon); }

/*!
    \brief      返回关闭按钮图标.
    \sa         setCloseButtonIcon
*/
const QPixmap& QCtmMessageTipView::closeButtonIcon() const { return m_impl->delegate->closeButtonIcon(); }

/*!
    \brief      设置为触控外观，即一直显示标题超链接样式和关闭按钮，默认为 false \a flag.
    \sa         touchControlStyle
*/
void QCtmMessageTipView::setTouchControlStyle(bool flag)
{
    m_impl->delegate->setTouchControlStyle(flag);
    m_impl->view->viewport()->update();
}

/*!
    \brief      返回是否为触控外观.
    \sa         setTouchControlStyle
*/
bool QCtmMessageTipView::touchControlStyle() const { return m_impl->delegate->touchControlStyle(); }

/*!
    \brief      清空所有消息.
*/
void QCtmMessageTipView::clearAll()
{
    if (emit aboutToClearAllMessages())
    {
        m_impl->model->clear();
    }
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
void QCtmMessageTipView::showEvent(QShowEvent* e) { QCtmAbstractMessageTipView::showEvent(e); }

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
    \brief      关闭 \a index 位置的消息.
*/
void QCtmMessageTipView::onCloseButtonClicked(const QModelIndex& index)
{
    if (!index.isValid())
        return;
    if (m_impl->model && emit aboutToCloseMessage(std::dynamic_pointer_cast<QCtmMessageTipData>(m_impl->model->message(index.row()))))
    {
        m_impl->model->removeMessage(m_impl->model->message(index.row()));
        m_impl->view->viewport()->update();
    }
}

/*!
    \brief      响应 \a index 位置的消息标题栏点击.
*/
void QCtmMessageTipView::onTitleClicked(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto msg = m_impl->model->message(index.row());
        emit messageClicked(std::dynamic_pointer_cast<QCtmMessageTipData>(msg));
    }
}
