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

#include "QCtmDialog.h"
#include "QCtmTitleBar.h"
#include "Private/QCtmFramelessDelegate_p.h"
#include "Private/QCtmFramelessDelegate_win.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QMoveEvent>
#include <QApplication>
#include <QScreen>

struct QCtmDialog::Impl
{
    QCtmTitleBar* title{ nullptr };
    QWidget* content{ nullptr };
    QVBoxLayout* layout{ nullptr };
#ifdef Q_OS_WIN
    QCtmWinFramelessDelegate* delegate{ nullptr };
#else
    QCtmFramelessDelegate* delegate{ nullptr };
#endif
};

/*!
    \class      QCtmDialog
    \brief      This class is the base class of top level widgets that can have a custom title bar.
    \inherits   QDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \fn         QCtmDialog::QCtmDialog(QWidget* parent)
    \brief      Constructs a dialog which is a child of \a parent.
*/
QCtmDialog::QCtmDialog(QWidget* parent)
    : QDialog(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->title = new QCtmTitleBar(this);
    m_impl->title->setObjectName("ctmDialogTitleBar");

    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setMargin(0);
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
    auto content = new QWidget(this);
    content->setAutoFillBackground(true);
    setCentralWidget(content);
}

/*!
    \brief      Destroys the widget.
*/
QCtmDialog::~QCtmDialog()
{
}

/*!
    \fn         void QCtmDialog::setCentralWidget(QWidget *widget)
    \brief      Sets the given widget to be the main window's central \a widget.
    \sa centralWidget
 */
void QCtmDialog::setCentralWidget(QWidget* widget)
{
    delete m_impl->content;
    m_impl->content = widget;
    widget->setAutoFillBackground(true);
    layout()->addWidget(widget);
    QVBoxLayout* l = qobject_cast<QVBoxLayout*>(layout());
    if (l)
    {
        l->setStretch(0, 0);
        l->setStretch(1, 1);
    }
    setWindowTitle(widget->windowTitle());
    m_impl->content->installEventFilter(this);
}

/*!
    \fn         QWidget* QCtmDialog::centralWidget() const
    \brief      Returns the central widget.
    \sa         setCentralWidget
*/
QWidget* QCtmDialog::centralWidget() const
{
    return m_impl->content;
}

/*!
    \brief      Sets the given \a titleBar, the old title bar will be delete.
    \sa         titleBar(), removeTitleBar()
*/
void QCtmDialog::setTitleBar(QCtmTitleBar* titleBar)
{
    if (m_impl->title)
    {
        m_impl->layout->replaceWidget(m_impl->title, titleBar);
        delete m_impl->title;
    }
    else
    {
        m_impl->layout->insertWidget(0, titleBar);
    }
    m_impl->delegate->addMoveBar(titleBar);
    m_impl->title = titleBar;
}

/*!
    \brief      Returns the title bar widget.
    \sa         setTitleBar, removeTitleBar()
*/
QCtmTitleBar* QCtmDialog::titleBar() const
{
    return m_impl->title;
}

/*!
    \brief      Remove the title bar, the title bar will be delete.
    \sa         setTitleBar, titleBar()
*/
void QCtmDialog::removeTitleBar()
{
    if (m_impl->title)
    {
        delete m_impl->title;
        m_impl->title = nullptr;
    }
}

/*!
    \brief      Add the given \a moveBar.
                The move bar provide a function that user can drag the move bar to move the dialog.
    \sa         removeMoveBar
*/
void QCtmDialog::addMoveBar(QWidget* moveBar)
{
    m_impl->delegate->addMoveBar(moveBar);
}

/*!
    \brief      Remove the give \a moveBar.
    \sa         addMoveBar
*/
void QCtmDialog::removeMoveBar(QWidget* moveBar)
{
    m_impl->delegate->removeMoveBar(moveBar);
}
#ifndef Q_OS_WIN

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Sets the window being shadowless, \a flag.
    \sa         shadowless()
    \else
    \internal
    \endif
*/
void QCtmDialog::setShadowless(bool flag)
{
    m_impl->delegate->setShadowless(flag);
}

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Returns the window is shadowless.
    \sa         setShadowless
    \else
    \internal
    \endif
*/
bool QCtmDialog::shadowless() const
{
    return m_impl->delegate->shadowless();
}
#endif

/*!
    \reimp
*/
void QCtmDialog::hideEvent(QHideEvent*)
{
    auto closeBtn = m_impl->title->findChild<QWidget*>("closeBtn");
    if (closeBtn)
    {
        auto e = new QEvent(QEvent::Type::Leave);
        qApp->sendEvent(closeBtn, e);
    }
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
bool QCtmDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
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

/*!
    \brief      Normalize \a pos
*/
void QCtmDialog::normalizes(QPoint& pos)
{
    auto screen = qApp->screenAt(pos);
    if (!screen)
        return;
    auto rect = screen->geometry();
    if (pos.x() < rect.left())
    {
        pos.setX(rect.left());
    }

    if (pos.x() + this->width() > rect.right())
    {
        pos.setX(rect.right() - this->width());
    }

    if (pos.y() < rect.top())
    {
        pos.setX(rect.top());
    }

    if (pos.y() + this->height() > rect.bottom())
    {
        pos.setY(rect.bottom() - this->height());
    }
}
