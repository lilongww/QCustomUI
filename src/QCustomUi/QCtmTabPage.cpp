#include "QCtmTabPage.h"
#include "QCtmTabWidget.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QActionEvent>

#include <assert.h>

struct QCtmTabPage::Impl
{
	QList<QCtmWidgetItemPtr> actions;
	QCtmTabWidget* parent{ nullptr };
	QWidget* cornerWidget{ nullptr };
	QHBoxLayout* cornerLayout{ nullptr };
	QHBoxLayout* layout{ nullptr };
	QWidget* centralWidget{ nullptr };
};

QAction* QCtmTabPage::addAction(const QIcon& icon, const QString& text)
{
	return insertAction(count(), icon, text);
}

QAction* QCtmTabPage::addAction(const QString& text)
{
	return addAction(QIcon(), text);
}


QAction* QCtmTabPage::insertAction(int index, const QIcon& icon, const QString& text)
{
    auto action = new QAction(icon, text, m_impl->cornerWidget);
	insertAction(index, action);
	return action;
}

void QCtmTabPage::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

QAction* QCtmTabPage::insertAction(int index, const QString& text)
{
	return insertAction(index, QIcon(), text);
}

QAction* QCtmTabPage::actionAt(int index) const
{
    if (index < 0)
        return nullptr;
    if (m_impl->actions.size() > index)
    {
        return m_impl->actions.at(index)->action();
    }
    return nullptr;
}

QWidget* QCtmTabPage::takeCentralWidget() const
{
	if (m_impl->layout->count() > 0)
	{
		auto item = m_impl->layout->takeAt(0);
		if (item->widget())
		{
			item->widget()->setParent(0);
			item->widget()->hide();
			return item->widget();
		}
	}
	return nullptr;
}

QWidget* QCtmTabPage::centralWidget() const
{
	return m_impl->centralWidget;
}

int QCtmTabPage::count() const
{
	return m_impl->actions.size();
}

void QCtmTabPage::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCtmTabPage::actionEvent(QActionEvent* event)
{
    if (event->type() == QActionEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
        Util::addItem(item, m_impl->actions, event->before(), m_impl->cornerLayout);
    }
    else if (event->type() == QActionEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->actions, m_impl->cornerLayout);
    }
}

QWidget* QCtmTabPage::cornerWidget() const
{
	return m_impl->cornerWidget;
}

void QCtmTabPage::setCentralWidget(QWidget* widget)
{
	m_impl->centralWidget = widget;
	m_impl->layout->addWidget(widget);
	widget->show();
}

QCtmTabPage::QCtmTabPage(QCtmTabWidget* parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	assert(parent);
	m_impl->cornerWidget = new QWidget;
	m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
	m_impl->cornerLayout->setMargin(0);

	m_impl->layout = new QHBoxLayout(this);
}

QCtmTabPage::~QCtmTabPage()
{
	m_impl->actions.clear();
	delete m_impl->cornerWidget;
}
