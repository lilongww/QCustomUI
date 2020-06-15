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

#include "QCtmToolBox.h"
#include "Private/QCtmToolBoxItem_p.h"

#include <QSplitter>
#include <QHBoxLayout>

struct QCtmToolBox::Impl
{
	QSplitter* vSplitter;
	bool firstShow{ true };
};

QCtmToolBox::QCtmToolBox(QWidget* parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->vSplitter = new QSplitter(this);
	m_impl->vSplitter->setOrientation(Qt::Vertical);
	m_impl->vSplitter->setChildrenCollapsible(false);
    m_impl->vSplitter->setObjectName("toolBoxSplitter");
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->addWidget(m_impl->vSplitter);
}

QCtmToolBox::~QCtmToolBox()
{

}

/**
 * @brief      Add a widget.
 */
void QCtmToolBox::addWidget(const QString& title, QWidget* widget)
{
	insertWidget(m_impl->vSplitter->count(), title, widget);
}

/**
 * @brief      Insert a widget.
 */
void QCtmToolBox::insertWidget(int index, const QString& title, QWidget* widget)
{
	auto item = new QCtmToolBoxItem(this);
	item->setTitle(title);
	item->setContent(widget);
	m_impl->vSplitter->insertWidget(index, item);
}

/**
 * @brief  		Remove the widget.
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

/**
 * @brief  		Remove the widget.
 */
void QCtmToolBox::removeWidget(int index)
{
	auto w = m_impl->vSplitter->widget(index);
	if (w)
		delete w;
}

/**
 * @brief  		Get the index of the given widget.
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

/**
 * @brief  		Get the widget of index.
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

/**
 * @brief  		Set the stretch factor of widgets.
 */
void QCtmToolBox::setStretchFactor(int index, int stretch)
{
	m_impl->vSplitter->setStretchFactor(index, stretch);
}

/**
 * @brief  		Sets the sizes of widgets.
 */
void QCtmToolBox::setSizes(const QList<int>& sizes)
{
	m_impl->vSplitter->setSizes(sizes);
}

/**
 *  @brief      Set default stretch factor of widgets.
 */
void QCtmToolBox::setDefaultStretch(int index, int stretch)
{
	auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(index));
	if (item)
	{
		item->setStretch(stretch);
	}
}

void QCtmToolBox::showEvent([[maybe_unused]] QShowEvent *event)
{
	if (m_impl->firstShow)
	{
		m_impl->firstShow = false;
		QList<int> sizes;
		float total = 0;
		for (int i = 0;i<m_impl->vSplitter->count();i++)
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
                    auto size = item->stretch() / total* this->height();
					sizes.push_back(size > item->minimumSizeHint().height() ? size : item->minimumHeight());
				}
			}
            m_impl->vSplitter->setSizes(sizes);
		}
	}
}

