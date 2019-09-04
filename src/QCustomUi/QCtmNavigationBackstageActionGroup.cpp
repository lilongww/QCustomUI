#include "QCtmNavigationBackstageActionGroup.h"
#include "QCtmNavigationBackstageMenu.h"
#include "Private/Util_p.h"
#include "Private/QCtmWidgetItem_p.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QActionEvent>

struct QCtmNavigationBackstageActionGroup::Impl
{
	QList<QCtmWidgetItemPtr> actions;
	QVBoxLayout* layout{ nullptr };
	QLabel* title;
	int itemHeight{ 50 };
};

QCtmNavigationBackstageActionGroup::QCtmNavigationBackstageActionGroup(const QString& groupName, QCtmNavigationBackstageMenu  *parent)
	: QFrame(parent)
	, m_impl(std::make_unique<Impl>())
{
	m_impl->layout = new QVBoxLayout(this);
	m_impl->title = new QLabel(this);
	m_impl->title->setText(groupName);
	m_impl->title->setObjectName("title");
	m_impl->layout->addWidget(m_impl->title);
	m_impl->layout->setAlignment(Qt::AlignTop);
}

QCtmNavigationBackstageActionGroup::~QCtmNavigationBackstageActionGroup()
{
}

QAction* QCtmNavigationBackstageActionGroup::addAction(const QString& text)
{
	return addAction(QIcon(), text);
}

QAction* QCtmNavigationBackstageActionGroup::addAction(const QIcon& icon, const QString& text)
{
	return insertAction(this->count(), icon, text);
}


bool QCtmNavigationBackstageActionGroup::setGroupName(const QString& groupName)
{
	m_impl->title->setText(groupName);
	return true;
}

QString QCtmNavigationBackstageActionGroup::groupName() const
{
	return m_impl->title->text();
}

QAction* QCtmNavigationBackstageActionGroup::insertAction(int index, const QIcon& icon, const QString& text)
{
	auto action = new QAction(icon, text, this);
	insertAction(index, action);
	return action;
}

void QCtmNavigationBackstageActionGroup::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

QAction* QCtmNavigationBackstageActionGroup::insertAction(int index, const QString& text)
{
	return insertAction(index, QIcon(), text);
}

void QCtmNavigationBackstageActionGroup::actionEvent(QActionEvent *event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        QCtmWidgetItemPtr item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Vertical, this);
        auto it = std::find_if(m_impl->actions.begin(), m_impl->actions.end(), [=](QCtmWidgetItemPtr item) { return item->action() == event->before(); });
        it = m_impl->actions.insert(it, item);
        auto index = m_impl->actions.indexOf(*it);
        m_impl->layout->insertWidget(index+1, item->widget());
        if (item->isCustomWidget())
        {
            item->widget()->show();
        }
        else
        {
            auto btn = qobject_cast<QToolButton*>(item->widget());
            if (btn)
            {
                btn->setFixedHeight(m_impl->itemHeight);
                btn->setIconSize(QSize(m_impl->itemHeight, m_impl->itemHeight));
            }
        }
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        auto item = Util::find(event->action(), m_impl->actions);
        if (item)
        {
            m_impl->actions.removeOne(item);
            m_impl->layout->takeAt(m_impl->layout->indexOf(item->widget()));
        }
    }
}

int QCtmNavigationBackstageActionGroup::indexOf(QAction* action) const
{
    auto item = Util::find(action, m_impl->actions);
	return m_impl->actions.indexOf(item);
}

int QCtmNavigationBackstageActionGroup::count() const
{
	return m_impl->actions.size();
}

QAction* QCtmNavigationBackstageActionGroup::actionAt(int index) const
{
	if (m_impl->actions.size() <= index)
		return nullptr;
	return m_impl->actions.at(index)->action();
}
