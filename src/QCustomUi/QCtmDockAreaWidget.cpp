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
#include "QCtmDockAreaWidget.h"
#include "QCtmDockWidget.h"
#include "Private/QCtmDockTabWidget_p.h"

#include <QHBoxLayout>
#include <QEvent>

struct QCtmDockAreaWidget::Impl
{
    QCtmDockTabWidget* tabWidget;
};

/*!
    \class      QCtmDockAreaWidget
    \brief      窗口停靠区域.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDockAreaWidget.h
*/

/*!
    \brief      构造一个父窗口为 \a parent 的停靠区域对象.
*/
QCtmDockAreaWidget::QCtmDockAreaWidget(QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_impl->tabWidget = new QCtmDockTabWidget(this));
}

/*!
    \brief      销毁该停靠区域对象.
*/
QCtmDockAreaWidget::~QCtmDockAreaWidget()
{
}

/*!
    \brief      添加一个停靠窗口 \a dock.
    \sa         insertDockWidget
*/
void QCtmDockAreaWidget::addDockWidget(QCtmDockWidget* dock)
{
    insertDockWidget(count(), dock);
}

/*!
    \brief      在第 \a index 的位置插入一个停靠窗口 \a dock.
    \sa         addDockWidget
*/
void QCtmDockAreaWidget::insertDockWidget(int index, QCtmDockWidget* dock)
{
    if (contains(dock))
        return;
    m_impl->tabWidget->insertTab(index, dock, dock->windowTitle());
    dock->installEventFilter(this);
}

/*!
    \brief      移除停靠窗口 \a dock.
*/
void QCtmDockAreaWidget::removeDockWidget(QCtmDockWidget* dock)
{
    if (auto index = indexOf(dock); index != -1)
    {
        removeDockWidget(index);
    }
}

/*!
    \overload
                移除第 \a index 个停靠窗口.
    \sa         removeDockWidget
*/
void QCtmDockAreaWidget::removeDockWidget(int index)
{
    m_impl->tabWidget->removeTab(index);
}

/*!
    \brief      清除所有的停靠窗口.
*/
void QCtmDockAreaWidget::clear()
{
    m_impl->tabWidget->clear();
}

/*!
    \brief      返回第 \a index 位置的停靠窗口地址.
*/
QCtmDockWidget* QCtmDockAreaWidget::dockWidget(int index) const
{
    return qobject_cast<QCtmDockWidget*>(m_impl->tabWidget->widget(index));
}

/*!
    \brief      返回停靠窗口 \a dock　在该停靠区域中的位置.
*/
int QCtmDockAreaWidget::indexOf(QCtmDockWidget* dock) const
{
    return m_impl->tabWidget->indexOf(dock);
}

/*!
    \brief      返回停靠窗口 \a dock 是否在该停靠区域中.
*/
bool QCtmDockAreaWidget::contains(QCtmDockWidget* dock) const
{
    for (int i = 0; i < m_impl->tabWidget->count(); i++)
    {
        if (dock == m_impl->tabWidget->widget(i))
            return true;
    }
    return false;
}

/*!
    \brief      返回该停靠区域中的停靠窗口数量.
*/
int QCtmDockAreaWidget::count() const
{
    return m_impl->tabWidget->count();
}

/*!
    \brief      返回当前显示的停靠窗口索引.
*/
int QCtmDockAreaWidget::currentIndex() const
{
    return m_impl->tabWidget->currentIndex();
}

/*!
    \brief      返回当前显示的停靠窗口地址.
*/
QCtmDockWidget* QCtmDockAreaWidget::currentDockWidget() const
{
    return qobject_cast<QCtmDockWidget*>(m_impl->tabWidget->currentWidget());
}

/*!
    \brief      将其他停靠窗口区域 \a other 合并到该停靠区域中.
*/
void QCtmDockAreaWidget::merage(QCtmDockAreaWidget* other)
{
    while (other->count())
    {
        auto dock = other->dockWidget(0);
        other->removeDockWidget(0);
        addDockWidget(dock);
    }
}

/*!
    \reimp
*/
bool QCtmDockAreaWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (auto dock = qobject_cast<QCtmDockWidget*>(watched); dock)
    {
        switch (event->type())
        {
        case QEvent::WindowTitleChange:
            if (auto index = indexOf(dock); index != -1)
            {
                m_impl->tabWidget->setTabText(index, dock->windowTitle());
                if (currentIndex() == index)
                    setWindowTitle(dock->windowTitle());
            }
        default:
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}
