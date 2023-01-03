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

#include "QCtmToolBox.h"
#include "Private/QCtmToolBoxItem_p.h"

#include <QHBoxLayout>
#include <QSplitter>

struct QCtmToolBox::Impl
{
    QSplitter* vSplitter;
    bool firstShow { true };
    QSize iconSize { 16, 16 };
};

/*!
    \class      QCtmToolBox
    \brief      类似QToolBox的容器窗口，可分割多个页面区域.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmToolBox.h
*/

/*!
    \fn         void QCtmToolBox::iconSizeChanged(const QSize& size);
    \brief      当Action的图标大小发生改变时发送该信号 \a size.
    \sa         setIconSize
*/

/*!
    \brief      构造一个父窗口为 \a parent 的toolbox.
*/
QCtmToolBox::QCtmToolBox(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->vSplitter = new QSplitter(this);
    m_impl->vSplitter->setOrientation(Qt::Vertical);
    m_impl->vSplitter->setChildrenCollapsible(false);
    m_impl->vSplitter->setObjectName("toolBoxSplitter");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_impl->vSplitter);
    setAttribute(Qt::WA_StyledBackground);
}

/*!
    \brief      析构当前toolbox对象.
*/
QCtmToolBox::~QCtmToolBox() {}

/*!
    \brief      添加一个标题为 \a title 显示内容为 \a widget 的子窗口.
    \sa         insertWidget, removeWidget
*/
void QCtmToolBox::addWidget(const QString& title, QWidget* widget) { insertWidget(m_impl->vSplitter->count(), title, widget); }

/*!
    \brief      在 \a index 的位置插入一个标题为 \a title 显示内容为 \a widget 的子窗口.
    \sa         addWidget, removeWidget
*/
void QCtmToolBox::insertWidget(int index, const QString& title, QWidget* widget)
{
    auto item = new QCtmToolBoxItem(this);
    item->setIconSize(m_impl->iconSize);
    connect(this, &QCtmToolBox::iconSizeChanged, item, &QCtmToolBoxItem::setIconSize);
    item->setTitle(title);
    item->setContent(widget);
    m_impl->vSplitter->insertWidget(index, item);
}

/*!
    \brief      移除子窗口 \a widget.
    \sa         addWidget, insertWidget
*/
void QCtmToolBox::removeWidget(QWidget* widget)
{
    for (int i = 0; i < m_impl->vSplitter->count(); i++)
    {
        auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(i));
        if (item->content() == widget)
        {
            delete item;
        }
    }
}

/*!
    \brief      返回 \a widget 的序号.
    \sa         widget
*/
int QCtmToolBox::indexOf(QWidget* widget) const
{
    for (int i = 0; i < m_impl->vSplitter->count(); i++)
    {
        auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(i));
        if (item->content() == widget)
        {
            return i;
        }
    }
    return -1;
}

/*!
    \brief      返回第 \a index 位置的子窗口.
    \sa         indexOf
*/
QWidget* QCtmToolBox::widget(int index) const
{
    auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(index));
    if (item)
    {
        return item->content();
    }
    return nullptr;
}

/*!
    \brief      设置第 \a index 子窗口的缩放因子 \a stretch.
*/
void QCtmToolBox::setStretchFactor(int index, int stretch) { m_impl->vSplitter->setStretchFactor(index, stretch); }

/*!
    \brief      设置子窗口们的大小 \a sizes.
    \sa         sizes
*/
void QCtmToolBox::setSizes(const QList<int>& sizes) { m_impl->vSplitter->setSizes(sizes); }

/*!
    \brief      返回子窗口们的大小.
    \sa         setSizes
*/
QList<int> QCtmToolBox::sizes() const { return m_impl->vSplitter->sizes(); }

/*!
    \brief      设置 \a index 子窗口的默认缩放因子 \a stretch.
*/
void QCtmToolBox::setDefaultStretch(int index, int stretch)
{
    auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(index));
    if (item)
    {
        item->setStretch(stretch);
    }
}

/*!
    \brief      设置Action的图标大小 \a size.
    \sa         iconSize
*/
void QCtmToolBox::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(size);
}

/*!
    \brief      返回Action的图标大小.
    \sa         setIconSize
*/
const QSize& QCtmToolBox::iconSize() const { return m_impl->iconSize; }

/*!
    \reimp
*/
void QCtmToolBox::showEvent([[maybe_unused]] QShowEvent* event)
{
    if (m_impl->firstShow)
    {
        m_impl->firstShow = false;
        QList<int> sizes;
        float total = 0;
        for (int i = 0; i < m_impl->vSplitter->count(); i++)
        {
            auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(i));
            if (item)
            {
                total += item->stretch();
            }
        }
        if (total != 0)
        {
            for (int i = 0; i < m_impl->vSplitter->count(); i++)
            {
                auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(i));
                if (item)
                {
                    auto size = item->stretch() / total * this->height();
                    sizes.push_back(size > item->minimumSizeHint().height() ? size : item->minimumHeight());
                }
            }
            m_impl->vSplitter->setSizes(sizes);
        }
    }
    QWidget::showEvent(event);
}
