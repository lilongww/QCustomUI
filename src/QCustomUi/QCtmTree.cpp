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

#include "QCtmTree.h"
#include "Private/QCtmTreeItemTitle_p.h"
#include "Private/Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDebug>

struct QCtmTree::Impl
{
	QSplitter *splitter{ nullptr };
    bool exclusive{ false };
    bool resizeLator{ false };
    QList<int> sizes;
};

QCtmTree::QCtmTree(QWidget *parent)
	: QWidget(parent),
	m_impl(std::make_unique<Impl>())
{
	m_impl->splitter = new QSplitter(this);
	m_impl->splitter->setHandleWidth(0);
	m_impl->splitter->setOrientation(Qt::Vertical);
	m_impl->splitter->setChildrenCollapsible(false);
	m_impl->splitter->setContentsMargins(0, 0, 0, 0);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(m_impl->splitter);
	layout->setMargin(0);
}

QCtmTree::~QCtmTree()
{
}

/**
 * @brief	    添加一个子窗口
 * @param[in]	title 标题
 * @param[in]	widget 子窗口地址
 * @return:     item地址
 */
QCtmTreeItem* QCtmTree::addWidget(const QString& title, QWidget* widget)
{
	return insertWidget(count(), title, widget);
}

/**
 * @brief	    插入一个子窗口
 * @param[in]	index 要插入的位置
 * @param[in]	title 标题
 * @param[in]	widget 子窗口地址
 * @return:     item地址
 */
QCtmTreeItem* QCtmTree::insertWidget(int index, const QString& title, QWidget* widget)
{
    auto item = new QCtmTreeItem(title, this);
    connect(item, &QCtmTreeItem::expandChanged, this, &QCtmTree::onItemExpand);
    connect(item, &QCtmTreeItem::titleClicked, this, &QCtmTree::onItemClicked);
    item->setWidget(widget);
	m_impl->splitter->insertWidget(index, item);
    m_impl->sizes = m_impl->splitter->sizes();
	item->installEventFilter(this);
	doResize();
    return item;
}

/**
 * @brief	    移除一项
 * @param[in]	item 要移除的项
 */
void QCtmTree::removeItem(QCtmTreeItem* item)
{
	delete item;
}

/**
 * @brief	    获取项所在位置
 * @param[in]	item 要获取位置的项
 * @return: 	位置
 */
int QCtmTree::indexOf(QCtmTreeItem* item) const
{
	return m_impl->splitter->indexOf(item);
}

/**
 * @brief	    获取指定位置的项
 * @param[in]	index 指定位置
 * @return:	    项地址
 */
QCtmTreeItem* QCtmTree::item(int index) const
{
	return qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(index));
}

/**
 * @brief	    设置所有项的展开状态
 * @param[in]	expand true:展开，false:收拢
 */
void QCtmTree::setAllExpand(bool expand) const
{
	for (int i = 0; i<count();i++)
	{
		item(i)->setExpand(expand);
	}
}

/**
 * @brief	    获取项总数
 * @return: 	项总数
 */
int QCtmTree::count() const
{
	return m_impl->splitter->count();
}

/**
 * @brief		设置子项展开互斥，即只能有一项展开
 * @param[in]
 */
void QCtmTree::setExclusive(bool exclusive)
{
    m_impl->exclusive = exclusive;
}

/**
 * @brief		子项是否展开互斥
 */
bool QCtmTree::exclusive() const
{
    return m_impl->exclusive;
}

/**
 * @brief		设置每一个item的高度
 * @param[in]	sizes 每个item的高度
 */
void QCtmTree::setSizes(const QList<int>& sizes)
{
    m_impl->sizes = sizes;
    for (int i = 0; i < sizes.size(); i++)
    {
        auto item = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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

void QCtmTree::doResize()
{
	auto sizes = m_impl->splitter->sizes();

	if (allClosed())
	{
		for (int i = 0;i<sizes.size();i++)
		{
			auto w = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
			sizes[i] = w->minimumSizeHint().height();
		}
		sizes.last() = this->height();
		m_impl->splitter->setSizes(sizes);
		return;
	}
	
	for (int i = 0; i < sizes.size(); i++)
	{
		auto w = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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
		auto w = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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

void QCtmTree::onItemExpand(bool expand)
{
    auto item = qobject_cast<QCtmTreeItem*>(sender());
    if (item && expand && m_impl->exclusive)
    {
        for (int i = 0; i < m_impl->splitter->count(); i++)
        {
            auto it = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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

void QCtmTree::onItemClicked(bool expand)
{
    auto item = qobject_cast<QCtmTreeItem*>(sender());
    if (item)
    {
        m_impl->sizes = this->m_impl->splitter->sizes();
        emit itemTitleClicked(item, expand);
    }
}

bool QCtmTree::allClosed() const
{
	for (int i = 0; i<m_impl->splitter->count();i++)
	{
		auto w = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
		if (w->isExpand())
			return false;
	}
	return true;
}

int QCtmTree::total(const QList<int>& sizes) const
{
	int total = 0;
	for (auto size : sizes)
	{
		total += size;
	}
	return total;
}

void QCtmTree::showEvent(QShowEvent* event)
{
    if (m_impl->resizeLator)
    {
        m_impl->splitter->setSizes(m_impl->sizes);
        m_impl->resizeLator = false;
    }
    QWidget::showEvent(event);
}

void QCtmTree::childExpandChanged(QCtmTreeItem* item, bool expand, [[maybe_unused]] int size)
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
                for (int i = index+1; i < sizes.size(); i++)
                {
                    if (s <= 0)
                        break;
                    auto item = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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
                    auto item = qobject_cast<QCtmTreeItem*>(m_impl->splitter->widget(i));
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

struct QCtmTreeItem::Impl
{
	QCtmTreeItemTitle* title{ nullptr };
	QVBoxLayout* layout{ nullptr };
	QScrollArea* content{ nullptr };
	QWidget* widget{ nullptr };
	int size = 0;
    QCtmTree* tree{ nullptr };
};

QCtmTreeItem::QCtmTreeItem(const QString& title, QCtmTree* parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
    m_impl->tree = parent;
	m_impl->layout = new QVBoxLayout(this);
	m_impl->layout->setMargin(0);
	m_impl->layout->setSpacing(0);
	
	m_impl->title = new QCtmTreeItemTitle(this);
	m_impl->title->setText(title);
	m_impl->title->setObjectName("title");

	m_impl->layout->addWidget(m_impl->title, 0);
	m_impl->content = new QScrollArea(this);
	m_impl->content->setWidgetResizable(true);
	m_impl->content->setFrameStyle(QFrame::NoFrame);
    m_impl->content->hide();
	m_impl->layout->addWidget(m_impl->content, 1);
	m_impl->title->setFixedHeight(30);

	connect(m_impl->title, &QCtmTreeItemTitle::clicked, this, &QCtmTreeItem::onClicked);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	m_impl->layout->setAlignment(m_impl->title, Qt::AlignTop);
	setExpand(false);
}

QCtmTreeItem::~QCtmTreeItem()
{

}

void QCtmTreeItem::setWidget(QWidget* widget)
{
	if (m_impl->widget)
		delete m_impl->widget;
	m_impl->widget = widget;
	m_impl->content->setWidget(widget);
    m_impl->title->update();
    if (!m_impl->widget)
        setExpand(false);
}

QWidget* QCtmTreeItem::widget() const
{
	return m_impl->widget;
}

void QCtmTreeItem::setTitle(const QString& title)
{
	m_impl->title->setText(title);
}

QString QCtmTreeItem::title() const
{
	return m_impl->title->text();
}

bool QCtmTreeItem::isExpand() const
{
	return m_impl->title->isExpand();
}

void QCtmTreeItem::setExpand(bool expand)
{
    if (isExpand() == expand)
        return;
    if (!m_impl->widget)
        return;
	m_impl->title->setExpand(expand);
	m_impl->content->setVisible(expand);
    if (m_impl->tree)
    {
        emit this->expandChanged(expand);
        this->metaObject()->invokeMethod(m_impl->tree
            , "childExpandChanged"
            , Qt::QueuedConnection
            , Q_ARG(QCtmTreeItem*, this)
            , Q_ARG(bool, expand)
            , Q_ARG(int, m_impl->size));
    }
	if (!expand)
		m_impl->size = this->height();
}

void QCtmTreeItem::onClicked()
{
    emit(titleClicked(!isExpand()));
    if(m_impl->widget)
	    setExpand(!isExpand());
}

void QCtmTreeItem::resizeEvent([[maybe_unused]] QResizeEvent *event)
{
    if (!isExpand())
    {
        if (m_impl->tree)
        {
            metaObject()->invokeMethod(m_impl->tree, "doResize", Qt::QueuedConnection);
        }
    }
}

int QCtmTreeItem::suggestSize() const
{
    return m_impl->size < this->minimumSizeHint().height() ? this->minimumSizeHint().height() : m_impl->size;
}

void QCtmTreeItem::setSuggestSize(int size)
{
    m_impl->size = size;
}

QAction* QCtmTreeItem::addAction(const QString& text)
{
    return insertAction(count(), text);
}

QAction* QCtmTreeItem::addAction(const QIcon& icon, const QString& text)
{
    return insertAction(count(), icon, text);
}

void QCtmTreeItem::insertAction(int index, QAction* action)
{
    m_impl->title->insertAction(index, action);
}

QAction* QCtmTreeItem::insertAction(int index, const QString& text)
{
    return insertAction(index, QIcon(), text);
}

QAction* QCtmTreeItem::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->title);
    insertAction(index, action);
    return action;
}

int QCtmTreeItem::count() const
{
    return m_impl->title->actions().size();
}

int QCtmTreeItem::indexOf(QAction* action)
{
    return m_impl->title->indexOf(action);
}

QAction* QCtmTreeItem::actionAt(int index)
{
    return m_impl->title->actionAt(index);
}
