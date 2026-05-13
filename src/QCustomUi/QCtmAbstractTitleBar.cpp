/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2026 LiLong                                              **
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
#include "QCtmAbstractTitleBar.h"

#include <QEvent>

/*!
    \class      QCtmAbstractTitleBar
    \brief      QCtmWindow 和 QCtmDialog 的抽象标题栏.
    \note       最大化，最小化，关闭按钮等显示控制由 QWidget::windowFlags 决定,
                标题栏文字由centralWidget的QWidget::windowTitle决定.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmAbstractTitleBar.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmAbstractTitleBar::QCtmAbstractTitleBar(QWidget* parent) : QWidget(parent)
{
}

/*!
    \brief      析构函数.
*/
QCtmAbstractTitleBar::~QCtmAbstractTitleBar() = default;

/*!
    \fn         void QCtmAbstractTitleBar::onWindowMaximized(bool isMaximumSized)
    \brief      在窗口最大化状态发生变化时调用,
                \a isMaximumSized 为 \c true 表示当前窗口已最大化，否则表示未最大化.

    该函数为纯虚函数，需要子类根据窗口是否处于最大化状态更新标题栏界面。
*/

/*!
    \fn         void QCtmAbstractTitleBar::onWindowMaximizeButtonHint(bool showMaximizeButton)
    \brief      在最大化按钮显示状态发生变化时调用,
                \a showMaximizeButton 为 \c true 表示应显示最大化按钮，否则表示不显示.

    该函数为纯虚函数，需要子类根据窗口标志更新最大化按钮的可见状态。
*/

/*!
    \fn         void QCtmAbstractTitleBar::onWindowCloseButtonHint(bool showCloseButton)
    \brief      在关闭按钮显示状态发生变化时调用.
    \a          showCloseButton 为 \c true 表示应显示关闭按钮，否则表示不显示.

    该函数为纯虚函数，需要子类根据窗口标志更新关闭按钮的可见状态。
*/

/*!
    \fn         void QCtmAbstractTitleBar::onWindowMinimizeButtonHint(bool showMinimizeButton)
    \brief      在最小化按钮显示状态发生变化时调用.
    \a          showMinimizeButton 为 \c true 表示应显示最小化按钮，否则表示不显示.

    该函数为纯虚函数，需要子类根据窗口标志更新最小化按钮的可见状态。
*/

/*!
    \fn         void QCtmAbstractTitleBar::onWindowTitleChanged(const QString& title)
    \brief      在窗口标题发生变化时调用.
    \a          title 当前窗口标题文本.

    该函数为纯虚函数，需要子类将窗口标题同步到标题栏显示区域。
*/

/*!
    \fn         bool QCtmAbstractTitleBar::showIconSystemMenu(const QPoint& pos) const
    \brief      显示系统图标菜单.
    \a          pos 系统菜单弹出位置.
                如果成功显示系统菜单则返回 \c true，否则返回 \c false.

    该函数为纯虚函数，需要子类在指定位置显示系统菜单。
*/

/*!
    \reimp
*/
void QCtmAbstractTitleBar::showEvent(QShowEvent* event)
{
    auto w = qobject_cast<QWidget*>(this->parent());
    if (!w)
        return;
    onWindowMaximized(w->windowState().testFlag(Qt::WindowState::WindowMaximized));
    onWindowMaximizeButtonHint(w->windowFlags().testFlag(Qt::WindowType::WindowMaximizeButtonHint));
    onWindowCloseButtonHint(w->windowFlags().testFlag(Qt::WindowType::WindowCloseButtonHint));
    onWindowMinimizeButtonHint(w->windowFlags().testFlag(Qt::WindowType::WindowMinimizeButtonHint));
}

/*!
    \reimp
*/
bool QCtmAbstractTitleBar::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == parent())
    {
        auto w = qobject_cast<QWidget*>(this->parent());
        if (!w)
            return false;
        if (event->type() == QEvent::WindowStateChange)
        {
            onWindowMaximized(w->windowState().testFlag(Qt::WindowState::WindowMaximized));
        }
        else if (event->type() == QEvent::WindowTitleChange)
        {
            onWindowTitleChanged(w->windowTitle());
        }
        else if (event->type() == QEvent::Resize)
        {
            onWindowMaximizeButtonHint(w->windowFlags().testFlag(Qt::WindowType::WindowMaximizeButtonHint));
        }
    }
    return false;
}

/*!
    \brief      关闭按钮被点击时的槽函数.
*/
void QCtmAbstractTitleBar::onCloseButtonClicked()
{
    auto w = qobject_cast<QWidget*>(this->parent());
    if (w)
        w->close();
}

/*!
    \brief      最大化按钮被点击时的槽函数.
*/
void QCtmAbstractTitleBar::onMaximizeButtonClicked()
{
    auto delegate = this->window()->findChild<QObject*>("qcustomui_frameless_delegate");
    if (!delegate)
        return;
    if (this->window()->isMaximized() || this->window()->isFullScreen())
        QMetaObject::invokeMethod(delegate, "showNormal");
    else
        QMetaObject::invokeMethod(delegate, "showMaximized");
}

/*!
    \brief      最小化按钮被点击时的槽函数.
*/
void QCtmAbstractTitleBar::onMinimizeButtonClicked()
{
    auto delegate = this->window()->findChild<QObject*>("qcustomui_frameless_delegate");
    if (!delegate)
        return;
    QMetaObject::invokeMethod(delegate, "showMinimized");
}
