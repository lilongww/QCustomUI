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
 * @brief      添加一个子窗口
 * @param[in]  title 标题
 * @param[in] 	widget 子窗口地址
 */
void QCtmToolBox::addWidget(const QString& title, QWidget* widget)
{
	insertWidget(m_impl->vSplitter->count(), title, widget);
}

/**
 * @brief      插入一个子窗口
 * @param[in]  index 插入位置
 * @param[in] 	title 标题
 * @param[in] 	widget 子窗口地址
 */
void QCtmToolBox::insertWidget(int index, const QString& title, QWidget* widget)
{
	auto item = new QCtmToolBoxItem(this);
	item->setTitle(title);
	item->setContent(widget);
	m_impl->vSplitter->insertWidget(index, item);
}

/**
 * @brief  		移除子窗口
 * @param[in]  	widget 子窗口地址
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
 * @brief  		移除子窗口
 * @param[in]  	index 子窗口位置
 */
void QCtmToolBox::removeWidget(int index)
{
	auto w = m_impl->vSplitter->widget(index);
	if (w)
		delete w;
}

/**
 * @brief  		查询子窗口位置
 * @param[in]  	widget 子窗口地址
 * @Return:   	子窗口位置
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
 * @brief  		查询指定位置的子窗口
 * @param[in]  	index 指定位置
 * @Return:   	子窗口地址
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
 * @brief  		设置子窗口间默认显示比例
 * @param[in]  	index 子窗口位置
 * @param[in] 	stretch 子窗口显示比重
 */
void QCtmToolBox::setStretchFactor(int index, int stretch)
{
	m_impl->vSplitter->setStretchFactor(index, stretch);
}

/**
 * @brief  		设置各子窗口的显示大小(高度)
 * @param[in]  	sizes 从上到下的大小集合
 */
void QCtmToolBox::setSizes(const QList<int>& sizes)
{
	m_impl->vSplitter->setSizes(sizes);
}

/**
 *  @brief      设置窗口第一次显示时的比例
 *  @param[in]  index 窗口序号
 *  @param[in] 	stretch 比例
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

