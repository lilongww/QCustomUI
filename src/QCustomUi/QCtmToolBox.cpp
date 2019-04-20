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
	, m_impl(std::make_shared<Impl>())
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

void QCtmToolBox::addWidget(const QString& title, QWidget* widget)
{
	insertWidget(m_impl->vSplitter->count(), title, widget);
}

void QCtmToolBox::insertWidget(int index, const QString& title, QWidget* widget)
{
	auto item = new QCtmToolBoxItem(this);
	item->setTitle(title);
	item->setContent(widget);
	m_impl->vSplitter->insertWidget(index, item);
}

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

void QCtmToolBox::removeWidget(int index)
{
	auto w = m_impl->vSplitter->widget(index);
	if (w)
		delete w;
}

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

QWidget* QCtmToolBox::widget(int index) const
{
	auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(index));
	if (item)
	{
		return item->content();
	}
	return nullptr;
}

void QCtmToolBox::setStretchFactor(int index, int stretch)
{
	m_impl->vSplitter->setStretchFactor(index, stretch);
}

void QCtmToolBox::setSizes(const QList<int>& sizes)
{
	m_impl->vSplitter->setSizes(sizes);
}

void QCtmToolBox::setDefaultStretch(int index, int stretch)
{
	auto item = qobject_cast<QCtmToolBoxItem*>(m_impl->vSplitter->widget(index));
	if (item)
	{
		item->setStretch(stretch);
	}
}

void QCtmToolBox::showEvent(QShowEvent *event)
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

