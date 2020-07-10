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

#include "QCtmDrawerWidget.h"
#include "QCtmDrawerItemWidget.h"
#include "Private/Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDebug>

struct QCtmDrawerWidget::Impl
{
    QSplitter* splitter{ nullptr };
    bool exclusive{ false };
    bool resizeLator{ false };
    QList<int> sizes;
};

/*!
    \class      QCtmDrawerWidget
    \brief      QCtmDrawerWidget provide a drawer widget container.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi

    \b          {The drawer widget screenshot:}
    \image      QCtmDrawerWidgetDetail.png
*/

/*!
    \property   QCtmDrawerWidget::exclusive
    \brief      This property holds the items expand state of the draw widget is exclusive.
*/

/*!
    \fn         void QCtmDrawerWidget::itemExpandChanged(QCtmDrawerItemWidget* item, bool expand)
    \brief      Emit this signal when the \a item \a expand state is changed.
    \sa         itemTitleClicked
*/

/*!
    \fn         void QCtmDrawerWidget::itemTitleClicked(QCtmDrawerItemWidget* item, bool expand)
    \brief      Emit this signal when the title bar of the \a item has being clicked, \a expand.
    \sa         itemExpandChanged
*/

/*!
    \brief      Constructs a drawer widget with the \a parent.
*/
QCtmDrawerWidget::QCtmDrawerWidget(QWidget* parent)
    : QWidget(parent),
    m_impl(std::make_unique<Impl>())
{
    m_impl->splitter = new QSplitter(this);
    m_impl->splitter->setHandleWidth(0);
    m_impl->splitter->setOrientation(Qt::Vertical);
    m_impl->splitter->setChildrenCollapsible(false);
    m_impl->splitter->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_impl->splitter);
    layout->setMargin(0);
    qRegisterMetaType<QCtmDrawerItemWidget*>("QCtmDrawerItemWidget*");
}

/*!
    \brief      Destroys the drawer widget.
*/
QCtmDrawerWidget::~QCtmDrawerWidget()
{
}

/*!
    \brief      Add the given \a widget to the drawer widget and set the given \a title text.
                Returns the item of the widget.
    \sa         insertWidget, removeItem
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::addWidget(const QString& title, QWidget* widget)
{
    return insertWidget(count(), title, widget);
}

/*!
    \brief      Insert the given \a widget to the drawer widget with \a index and set the given \a title text.
                Returns the item of the widget.
    \sa         addWidget, removeItem
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::insertWidget(int index, const QString& title, QWidget* widget)
{
    auto item = new QCtmDrawerItemWidget(title, this);
    connect(item, &QCtmDrawerItemWidget::expandChanged, this, &QCtmDrawerWidget::onItemExpand);
    connect(item, &QCtmDrawerItemWidget::titleClicked, this, &QCtmDrawerWidget::onItemClicked);
    item->setWidget(widget);
    m_impl->splitter->insertWidget(index, item);
    m_impl->sizes = m_impl->splitter->sizes();
    item->installEventFilter(this);
    doResize();
    return item;
}

/*!
    \brief      Removes the \a item from the drawer widget.
    \sa         addWidget, insertWidget
*/
void QCtmDrawerWidget::removeItem(QCtmDrawerItemWidget* item)
{
    delete item;
}

/*!
    \brief      Returns the index of the given \a item.
    \sa         item
*/
int QCtmDrawerWidget::indexOf(QCtmDrawerItemWidget* item) const
{
    return m_impl->splitter->indexOf(item);
}

/*!
    \brief      Returns the item of the given \a index.
    \sa         indexOf
*/
QCtmDrawerItemWidget* QCtmDrawerWidget::item(int index) const
{
    return qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(index));
}

/*!
    \brief      Returns count of the items.
*/
int QCtmDrawerWidget::count() const
{
    return m_impl->splitter->count();
}

/*!
    \brief      Sets the expand state whether to be \a exclusive.
    \sa         exclusive()
*/
void QCtmDrawerWidget::setExclusive(bool exclusive)
{
    m_impl->exclusive = exclusive;
}

/*!
    \brief      Returns the expand state whether to be exclusive.
    \sa         setExclusive
*/
bool QCtmDrawerWidget::exclusive() const
{
    return m_impl->exclusive;
}

/*!
    \brief      Sets the \a sizes of items.
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
    \brief      Calculate and resize the items.
*/
void QCtmDrawerWidget::doResize()
{
    auto sizes = m_impl->splitter->sizes();

    if (allClosed())
    {
        for (int i = 0; i < sizes.size(); i++)
        {
            auto w = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
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
                auto h = w->minimumSizeHint().height();
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
                sizes[i] += m_impl->splitter->height() - this->total(sizes);
                break;
            }
        }
    }

    m_impl->splitter->setSizes(sizes);
}

/*!
    \brief      Update the drawer widget when the \a expand state of item has been changed.
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
    \brief      Update the drawer widget when the title bar of the item clicked, \a expand.
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
    \brief      Expands all expandable items.
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
    \brief      Collapses all expanded items.
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
    \brief      Returns all of the items is closed.
*/
bool QCtmDrawerWidget::allClosed() const
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
    \brief      Returns the total \a sizes.
*/
int QCtmDrawerWidget::total(const QList<int>& sizes) const
{
    int total = 0;
    for (auto size : sizes)
    {
        total += size;
    }
    return total;
}

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
    \brief      Update the drawer widget when the \a expand state of \a item has been changed.
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
                auto total1 = total(sizes);
                sizes[index] = item->suggestSize();
                auto total2 = total(sizes);
                int s = total2 - total1;
                for (int i = index + 1; i < sizes.size(); i++)
                {
                    if (s <= 0)
                        break;
                    auto item = qobject_cast<QCtmDrawerItemWidget*>(m_impl->splitter->widget(i));
                    if (item->isExpand())
                    {
                        auto sub = item->height() - item->minimumSizeHint().height();
                        auto h = s >= sub ? sub : s;
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
                        auto h = s >= sub ? sub : s;
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
