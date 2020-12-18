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
#include "QCtmDockWidget.h"

#include <QHBoxLayout>
#include <QPointer>
#include <QEvent>

struct QCtmDockWidget::Impl
{
    QPointer<QWidget> widget{ nullptr };
    QHBoxLayout* layout{ nullptr };
};

/*!
    \class      QCtmDockWidget
    \brief      停靠窗口
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDockWidget.h
*/

/*!
    \brief      构造一个父窗口为 \a parent 的停靠窗口对象.
*/
QCtmDockWidget::QCtmDockWidget(QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->layout = new QHBoxLayout(this);
}

/*!
    \brief      销毁该停靠窗口对象.
*/
QCtmDockWidget::~QCtmDockWidget()
{
}

/*!
    \brief      设置停靠窗口显示的内容 \a widget.
    \sa         widget
*/
void QCtmDockWidget::setWidget(QWidget* widget)
{
    if (m_impl->widget)
    {
        m_impl->layout->removeWidget(m_impl->widget);
        m_impl->widget->removeEventFilter(this);
        setWindowTitle("");
    }
    m_impl->widget = widget;
    m_impl->widget->installEventFilter(this);
    m_impl->layout->addWidget(m_impl->widget);
    if (windowTitle().isEmpty())
    {
        setWindowTitle(widget->windowTitle());
    }
}

/*!
    \brief      返回停靠窗口显示的内容窗口.
*/
QWidget* QCtmDockWidget::widget() const
{
    return m_impl->widget;
}

/*!
    \reimp
*/
bool QCtmDockWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_impl->widget)
    {
        switch (event->type())
        {
        case QEvent::WindowTitleChange:
            setWindowTitle(m_impl->widget->windowTitle());
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}
