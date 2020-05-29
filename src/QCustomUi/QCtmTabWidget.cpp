#include "QCtmTabWidget.h"
#include "QCtmTabPage.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QEvent>
#include <QHBoxLayout>
#include <QToolButton>

struct QCtmTabWidget::Impl
{
	QWidget* cornerWidget{ nullptr };
	QHBoxLayout* cornerLayout{ nullptr };
	QToolButton* closeBtn{ nullptr };
	QToolButton* areaBtn{ nullptr };

	void setPageCornerWidget(QWidget* widget)
	{
		if (cornerLayout->itemAt(0))
		{
			if (cornerLayout->itemAt(0)->widget() != areaBtn)
			{
				auto item = cornerLayout->takeAt(0);
				if (item->widget())
					item->widget()->hide();
			}
		}
		cornerLayout->insertWidget(0, widget);
	}
}; 

QCtmTabWidget::QCtmTabWidget(QWidget *parent):
	QTabWidget(parent),
	m_impl(std::make_unique<Impl>())
{
	m_impl->cornerWidget = new QWidget(this);
	m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
	m_impl->cornerLayout->setMargin(0);

	m_impl->closeBtn = new QToolButton(this);
	m_impl->closeBtn->setObjectName("closeButton");
    m_impl->areaBtn = new QToolButton(this);
    m_impl->areaBtn->setObjectName("areaButton");
    m_impl->areaBtn->hide();

    m_impl->cornerLayout->addWidget(m_impl->areaBtn);
	m_impl->cornerLayout->addWidget(m_impl->closeBtn);

	setCornerWidget(m_impl->cornerWidget);
	connect(this, &QTabWidget::currentChanged, this, &QCtmTabWidget::onCurrentChanged);
	connect(m_impl->closeBtn, &QToolButton::clicked, this, &QWidget::hide);
	connect(m_impl->areaBtn, &QToolButton::clicked, this, &QCtmTabWidget::areaButtonClicked);

}


QCtmTabWidget::~QCtmTabWidget()
{

}

/**
 * @brief       添加Tab页
 * @param[in]   widget	页窗口
 * @param[in]   label	页标签文本
 * @Return:     添加页的索引
 */
QCtmTabPage* QCtmTabWidget::addTab(QWidget *widget, const QString &label)
{
	return addTab(widget, QIcon(), label);
}

/**
 * @brief		添加Tab页
 * @param[in]	widget	页窗口
 * @param[in]	icon	页标签图标
 * @param[in]	label	页标签文本
 * @Return:		添加页的索引
 */
QCtmTabPage* QCtmTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
	return insertTab(count(), widget, icon, label);
}

/**
 * @brief       插入Tab页
 * @param[in]   index 插入页的index
 * @param[in]   widget 页窗口
 * @param[in]   label 页标签文本
 * @Return:     插入页索引
 */
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget *widget, const QString &label)
{
	return insertTab(index, widget, QIcon(), label);
}

/**
 *  @brief		插入Tab页
 *  @param[in]	index 插入页的index
 *  @param[in]	widget 页窗口
 *  @param[in]	icon 页标签图标
 *  @param[in]	label 页标签文本
 *  @Return:	插入页索引
 */
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget *widget, [[maybe_unused]] const QIcon &icon, const QString &label)
{
	auto page = new QCtmTabPage(this);
	page->setCentralWidget(widget);
	QTabWidget::insertTab(index, page, label);
	return page;
}

void QCtmTabWidget::paintEvent([[maybe_unused]] QPaintEvent *e)
{
	QPainter p(this);
	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCtmTabWidget::onCurrentChanged(int index)
{
	auto w = this->widget(index);
	auto page = qobject_cast<QCtmTabPage*>(w);
	if (page)
	{
		m_impl->setPageCornerWidget(page->cornerWidget());
		page->cornerWidget()->show();
	}
	else
		this->setCornerWidget(nullptr);
}

