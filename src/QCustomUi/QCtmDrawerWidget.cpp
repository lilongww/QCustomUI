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

#include "QCtmDrawerWidget.h"
#include "Private/Util_p.h"
#include "QCtmDrawerItemWidget.h"

#include <QDebug>
#include <QPainter>
#include <QSplitter>
#include <QStyleOption>
#include <QVBoxLayout>

#include <algorithm>

struct QCtmDrawerWidget::Impl
{
    QSplitter* splitter { nullptr };
    bool exclusive { false };
    bool resizeLator { false };
    QList<int> sizes;
    QSize iconSize { 16, 16 };
};

/*!
    \class      QCtmDrawerWidget
    \brief      抽屉窗口类.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDrawerWidget.h

    \b          {抽屉窗口的截图:}
    \image      QCtmDrawerWidgetDetail.png
*/

/*!
    \property   QCtmDrawerWidget::exclusive
    \brief      该属性表示抽屉窗口的各项展开状态是否互斥.
*/

/*!
    \fn         void QCtmDrawerWidget::itemExpandChanged(QCtmDrawerItemWidget* item, bool expand)
    \brief      抽屉项 \a item 的展开状态 \a expand 发生变化时发送该信号.
    \sa         itemTitleClicked
*/

/*!
    \fn         void QCtmDrawerWidget::itemTitleClicked(QCtmDrawerItemWidget* item, bool expand)
    \brief      抽屉项 \a item 的标题被点击时发送该信号 \a expand.
    \sa         itemExpandChanged
*/

/*!
    \fn         void QCtmDrawerWidget::iconSizeChanged(const QSize& size);
    \brief      当Action的图标大小发生改变时发送该信号 \a size.
    \sa         setIconSize
*/

/*!
    \brief      构造一个父窗口为 \a parent 的抽屉窗口对象.
*/
QCtmDrawerWidget::QCtmDrawerWidget(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->splitter = new QSplitter(this);
    m_impl->splitter->setHandleWidth(0);
    m_impl->splitter->setOrientation(Qt::Vertical);
    m_impl->splitter->setChildrenCollapsible(false);
    m_impl->splitter->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_impl->splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    qRegisterMetaType<QCtmDrawerItemWidget*>("QCtmDrawerItemWidget*");
}

/*!
    \brief      销毁该抽屉窗口对象.
*/
QCtmDrawerWidget::~QCtmDrawerWidget() {}

/*!
    \brief      添加并返回一个标题为 \a title 容纳窗口为 \a widget 的抽屉项.
    \sa         insertWidget, removeItem
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::addWidget(const QString& title, QWidget* widget) { return insertWidget(count(), title, widget); }

/*!
    \brief      在 \a index 位置插入并返回一个标题为 \a title 容纳窗口为 \a widget 的抽屉项.
    \sa         addWidget, removeItem
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::insertWidget(int index, const QString& title, QWidget* widget)
{
    auto item = new QCtmDrawerItemWidget(title, this);
    item->setIconSize(m_impl->iconSize);
    connect(item, &QCtmDrawerItemWidget::expandChanged, this, &QCtmDrawerWidget::onItemExpand);
    connect(item, &QCtmDrawerItemWidget::titleClicked, this, &QCtmDrawerWidget::onItemClicked);
    connect(this, &QCtmDrawerWidget::iconSizeChanged, item, &QCtmDrawerItemWidget::setIconSize);
    item->setWidget(widget);
    m_impl->splitter->insertWidget(index, item);
    m_impl->sizes = m_impl->splitter->sizes();
    item->installEventFilter(this);
    doResize();
    return item;
}

/*!
    \brief      移除给予的抽屉项 \a item.
    \sa         addWidget, insertWidget
*/
void QCtmDrawerWidget::removeItem(QCtmDrawerItemWidget* item) { delete item; }

/*!
    \brief      返回抽屉项 \a item 所在的位置.
    \sa         item
*/
int QCtmDrawerWidget::indexOf(QCtmDrawerItemWidget* item) const { return m_impl->splitter->indexOf(item); }

/*!
    \brief      返回 \a index 位置的抽屉项.
    \sa         indexOf
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::item(int index) const
{
    return qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(index));
}

/*!
    \brief      返回抽屉项数量.
*/
int QCtmDrawerWidget::count() const { return m_impl->splitter->count(); }

/*!
    \brief      设置抽屉项的展开状态是否互斥 \a exclusive.
    \sa         exclusive()
*/
void QCtmDrawerWidget::setExclusive(bool exclusive) { m_impl->exclusive = exclusive; }

/*!
    \brief      返回抽屉项的展开状态是否互斥.
    \sa         setExclusive
*/
bool QCtmDrawerWidget::exclusive() const { return m_impl->exclusive; }

/*!
    \brief      设置各抽屉项的大小 \a sizes.
\*/
void QCtmDrawerWidget::setSizes(const QList<int>& sizes)
{
    m_impl->sizes = sizes;
    for (int i = 0; i < sizes.size(); i++)
    {
        auto item = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
        if (item)
        {
            item->setSuggestSize(sizes[i]);
        }
    }
    if (isVisible())
        m_impl->splitter->setSizes(sizes);
    else
        m_impl->resizeLator = true;
}

/*!
    \brief      设置Action的图标大小 \a size.
    \sa         iconSize
*/
void QCtmDrawerWidget::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(size);
}

/*!
    \brief      返回Action的图标大小.
    \sa         setIconSize
*/
const QSize& QCtmDrawerWidget::iconSize() const { return m_impl->iconSize; }
/*!
    \brief      计算各抽屉项的大小.
*/
void QCtmDrawerWidget::doResize()
{
    auto sizes = m_impl->splitter->sizes();

    if (isAllCollapsed())
    {
        for (int i = 0; i < sizes.size(); i++)
        {
            auto w   = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
            sizes[i] = w->minimumSizeHint().height();
        }
        sizes.last() = this->height();
        m_impl->splitter->setSizes(sizes);
        return;
    }

    for (int i = 0; i < sizes.size(); i++)
    {
        auto w = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
        if (w)
        {
            if (!w->isExpand())
            {
                auto h   = w->minimumSizeHint().height();
                sizes[i] = h;
            }
        }
    }
    for (int i = sizes.size() - 1; i >= 0; i--)
    {
        auto w = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
        if (w)
        {
            if (w->isExpand())
            {
                sizes[i] += m_impl->splitter->height() - this->totalSize(sizes);
                break;
            }
        }
    }

    m_impl->splitter->setSizes(sizes);
}

/*!
    \brief      响应抽屉项展开状态 \a expand.
    \sa         onItemClicked
*/
void QCtmDrawerWidget::onItemExpand(bool expand)
{
    auto item = qobject_cast<QCtmDrawerItemWidget*>(sender());
    if (item && expand && m_impl->exclusive)
    {
        for (int i = 0; i < m_impl->splitter->count(); i++)
        {
            auto it = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
            if (it && it != item)
            {
                it->setExpand(false);
            }
        }
    }
    if (item)
    {
        emit itemExpandChanged(item, expand);
    }
}

/*!
    \brief      响应抽屉项标题点击信号 \a expand.
    \sa         onItemExpand
*/
void QCtmDrawerWidget::onItemClicked(bool expand)
{
    auto item = qobject_cast<QCtmDrawerItemWidget*>(sender());
    if (item)
    {
        m_impl->sizes = this->m_impl->splitter->sizes();
        emit itemTitleClicked(item, expand);
    }
}

/*!
    \brief      展开所有抽屉项.
    \sa         collapseAll()
*/
void QCtmDrawerWidget::expandAll()
{
    for (int i = 0; i < count(); i++)
    {
        item(i)->setExpand(true);
    }
}

/*!
    \brief      关闭所有抽屉项.
    \sa         expandAll()
*/
void QCtmDrawerWidget::collapseAll()
{
    for (int i = 0; i < count(); i++)
    {
        item(i)->setExpand(false);
    }
}

/*!
    \brief      返回所有的抽屉项是否都关闭了.
*/
bool QCtmDrawerWidget::isAllCollapsed() const
{
    for (int i = 0; i < m_impl->splitter->count(); i++)
    {
        auto w = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
        if (w->isExpand())
            return false;
    }
    return true;
}

/*!
    \brief      返回 \a sizes 的总和.
*/
int QCtmDrawerWidget::totalSize(const QList<int>& sizes) const { return std::accumulate(sizes.begin(), sizes.end(), 0); }

/*!
    \reimp
*/
void QCtmDrawerWidget::showEvent(QShowEvent* event)
{
    if (m_impl->resizeLator)
    {
        m_impl->splitter->setSizes(m_impl->sizes);
        m_impl->resizeLator = false;
    }
    QWidget::showEvent(event);
}

/*!
    \brief      响应抽屉项 \a item 的展开状态变化 \a expand.
*/
void QCtmDrawerWidget::childExpandChanged(QCtmDrawerItemWidget* item, bool expand)
{
    if (expand)
    {
        if (item)
        {
            int index = m_impl->splitter->indexOf(item);
            if (index != -1)
            {
                QList<int> sizes = m_impl->sizes;
                {
                    int i = sizes.size();
                    while (sizes.size() < m_impl->splitter->count())
                        sizes << m_impl->splitter->widget(i)->height();
                }
                for (int i = 0; i < m_impl->splitter->count(); i++)
                {
                    m_impl->splitter->setStretchFactor(i, i == index ? 1 : 0);
                }
                auto total1  = totalSize(sizes);
                sizes[index] = item->suggestSize();
                auto total2  = totalSize(sizes);
                int s        = total2 - total1;
                for (int i = index + 1; i < sizes.size(); i++)
                {
                    if (s <= 0)
                        break;
                    auto item = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
                    if (item->isExpand())
                    {
                        auto sub = item->height() - item->minimumSizeHint().height();
                        auto h   = s >= sub ? sub : s;
                        s -= h;
                        sizes[i] -= h;
                    }
                }

                for (int i = index - 1; i >= 0; i--)
                {
                    if (s <= 0)
                        break;
                    auto item = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
                    if (item->isExpand())
                    {
                        auto sub = item->height() - item->minimumSizeHint().height();
                        auto h   = s >= sub ? sub : s;
                        s -= h;
                        sizes[i] -= h;
                    }
                }

                m_impl->splitter->setSizes(sizes);
            }
        }
    }
    doResize();
}
