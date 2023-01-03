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

#include "QCtmDialog.h"
#include "Private/QCtmFramelessDelegate_p.h"
#include "Private/QCtmFramelessDelegate_win.h"
#include "QCtmTitleBar.h"

#include <QApplication>
#include <QMoveEvent>
#include <QPointer>
#include <QScreen>
#include <QVBoxLayout>

struct QCtmDialog::Impl
{
    QPointer<QCtmTitleBar> title;
    QWidget* content { nullptr };
    QVBoxLayout* layout { nullptr };
#ifdef Q_OS_WIN
    QCtmWinFramelessDelegate* delegate { nullptr };
#else
    QCtmFramelessDelegate* delegate { nullptr };
#endif
};

/*!
    \class      QCtmDialog
    \brief      这是一个模态窗口，类似于QDialog，去掉了系统的边框和标题栏，用自定义的标题栏替代.
    \inherits   QDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDialog.h
*/

/*!
    \brief      构造一个父窗口为 \a parent 的窗口对象.
*/
QCtmDialog::QCtmDialog(QWidget* parent) : QDialog(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->title = new QCtmTitleBar(this);

    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setContentsMargins(0, 0, 0, 0);
    m_impl->layout->setSpacing(0);

    m_impl->layout->addWidget(m_impl->title);
    m_impl->layout->setStretch(0, 0);
    m_impl->layout->setStretch(1, 1);
    m_impl->layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

#ifdef Q_OS_WIN
    m_impl->delegate = new QCtmWinFramelessDelegate(this, m_impl->title);
#else
    m_impl->delegate = new QCtmFramelessDelegate(this, m_impl->title);
    setWindowFlag(Qt::Dialog);
#endif
    setCentralWidget(new QWidget(this));
}

/*!
    \brief      销毁该窗口对象.
*/
QCtmDialog::~QCtmDialog() {}

/*!
    \brief      设置 \a widget 为窗口的中央窗口, 当 \a widget 为nullptr时删除中央窗口.
    \sa         centralWidget
 */
void QCtmDialog::setCentralWidget(QWidget* widget)
{
    if (widget == m_impl->content)
        return;
    if (m_impl->content)
    {
        m_impl->content->hide();
        m_impl->content->deleteLater();
    }
    m_impl->content = widget;
    if (widget)
    {
        QVBoxLayout* l = qobject_cast<QVBoxLayout*>(layout());
        l->addWidget(widget, 1);
        setWindowTitle(widget->windowTitle());
        m_impl->content->installEventFilter(this);
    }
}

/*!
    \brief      返回中央窗口地址, 如果中央窗口不存在则返回nullptr.
    \sa         setCentralWidget
*/
QWidget* QCtmDialog::centralWidget() const { return m_impl->content; }

/*!
    \brief      设置标题栏 \a titleBar, 当 \a titleBar 为 nullptr 时删除标题栏.
    \sa         titleBar
*/
void QCtmDialog::setTitleBar(QCtmTitleBar* titleBar)
{
    if (m_impl->title == titleBar)
        return;

    if (m_impl->title)
    {
        m_impl->delegate->removeMoveBar(m_impl->title);
        m_impl->title->hide();
        m_impl->title->deleteLater();
    }
    m_impl->title = titleBar;
    if (titleBar)
    {
        m_impl->layout->insertWidget(0, titleBar, 0);
        m_impl->delegate->addMoveBar(titleBar);
    }
}

/*!
    \brief      返回窗口的标题栏，如果标题栏不存在，自动创建并返回一个新的标题栏.
    \sa         setTitleBar
*/
QCtmTitleBar* QCtmDialog::titleBar() const
{
    auto title = m_impl->title;
    if (!title)
    {
        auto self = const_cast<QCtmDialog*>(this);
        title     = new QCtmTitleBar(self);
        self->setTitleBar(title);
    }
    return title;
}

/*!
    \brief      添加鼠标可拖拽区域 \a moveBar, moveBar必须为该窗口的子控件之一.
    \sa         removeMoveBar
*/
void QCtmDialog::addMoveBar(QWidget* moveBar) { m_impl->delegate->addMoveBar(moveBar); }

/*!
    \brief      移除鼠标拖拽区域 \a moveBar.
    \sa         addMoveBar
*/
void QCtmDialog::removeMoveBar(QWidget* moveBar) { m_impl->delegate->removeMoveBar(moveBar); }
#ifndef Q_OS_WIN

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Sets the window being shadowless, \a flag.
    \sa         shadowless()
    \else
    \internal
    \endif
*/
void QCtmDialog::setShadowless(bool flag) { m_impl->delegate->setShadowless(flag); }

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Returns the window is shadowless.
    \sa         setShadowless
    \else
    \internal
    \endif
*/
bool QCtmDialog::shadowless() const { return m_impl->delegate->shadowless(); }
#endif

/*!
    \reimp
*/
void QCtmDialog::hideEvent(QHideEvent* e)
{
    auto closeBtn = m_impl->title->findChild<QWidget*>("closeBtn");
    if (closeBtn)
    {
        auto e = new QEvent(QEvent::Type::Leave);
        qApp->sendEvent(closeBtn, e);
    }
    QDialog::hideEvent(e);
}

/*!
    \reimp
*/
bool QCtmDialog::eventFilter(QObject* o, QEvent* e)
{
    if (o == m_impl->content && e->type() == QEvent::WindowTitleChange)
    {
        setWindowTitle(m_impl->content->windowTitle());
    }
    return QDialog::eventFilter(o, e);
}

/*!
    \reimp
*/
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QCtmDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#else
bool QCtmDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#endif
{
#ifdef Q_OS_WIN
    if (!m_impl->delegate)
        return QDialog::nativeEvent(eventType, message, result);
    if (!m_impl->delegate->nativeEvent(eventType, message, result))
        return QDialog::nativeEvent(eventType, message, result);
    else
        return true;
#else
    return QDialog::nativeEvent(eventType, message, result);
#endif
}
