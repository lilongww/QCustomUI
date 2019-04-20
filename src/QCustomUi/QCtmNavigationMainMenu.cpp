#include "QCtmNavigationMainMenu.h"
#include "QCtmNavigationBackstageMenu.h"
#include "QCtmColor.h"
#include "Private/flowlayout_p.h"
#include "Private/Util_p.h"
#include "Private/QCtmWidgetItemForMainMenu_p.h"
#include "Private/QCtmMainMenuButton_p.h"

#include <QToolButton>
#include <QMouseEvent>
#include <QAction>
#include <QStyleOption>
#include <QColor>
#include <QDebug>

QCtmWidgetItemForMainMenuPtr find(QAction* action, const QList<QCtmWidgetItemForMainMenuPtr>& items)
{
    for (auto &item : items)
    {
        if (item->action() == action)
            return item;
    }
    return nullptr;
}

QCtmWidgetItemForMainMenuPtr find(QWidget* widget, const QList<QCtmWidgetItemForMainMenuPtr>& items)
{
    for (auto &item : items)
    {
        if (item->widget() == widget)
            return item;
    }
    return nullptr;
}


struct QCtmNavigationMainMenu::Impl
{
	FlowLayout* layout{ nullptr };
	QList<QCtmWidgetItemForMainMenuPtr> items;
	QWidget* viewPort{ nullptr };

	int actionWidth{ 180 };
	int actionHeight{ 74 };

    bool colorAlternate{ true };
};

QCtmNavigationMainMenu::QCtmNavigationMainMenu(QCtmNavigationBar *parent)
	: QCtmNavigationSidePanel(parent)
	, m_impl(std::make_shared<Impl>())
{
	auto viewPort = new QWidget(this);
	m_impl->layout = new FlowLayout(viewPort);
	m_impl->layout->setMargin(20);
	m_impl->layout->setHorizontalSpacing(15);
	this->setWidget(viewPort);
	viewPort->setAutoFillBackground(false);
	setDockArea(DockArea::Top);
}

QCtmNavigationMainMenu::~QCtmNavigationMainMenu()
{
}

QAction* QCtmNavigationMainMenu::addAction(const QString& text)
{
	return addAction(QIcon(), text);
}

QAction* QCtmNavigationMainMenu::addAction(const QIcon& icon, const QString& text)
{
	return insertAction(count(), icon, text);
}

QAction* QCtmNavigationMainMenu::insertGroupingMenu(int index, const QIcon& icon, const QString& text, QCtmNavigationBackstageMenu * menu)
{
	auto action = insertAction(index, icon, text);
	bindMenu(action, menu);
	return action;
}

QAction* QCtmNavigationMainMenu::insertGroupingMenu(int index, const QString& text, QCtmNavigationBackstageMenu * menu)
{
	return insertGroupingMenu(index, QIcon(), text, menu);
}

void QCtmNavigationMainMenu::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

QAction* QCtmNavigationMainMenu::insertAction(int index, const QString& text)
{
	return insertAction(index, QIcon(), text);
}

QAction* QCtmNavigationMainMenu::insertAction(int index, const QIcon& icon, const QString& text)
{
	auto action = new QAction(icon, text, this);
	insertAction(index, action);
	return action;
}

void QCtmNavigationMainMenu::bindMenu(QAction* action, QCtmNavigationBackstageMenu * menu)
{
	action->setCheckable(true);
	connect(action, &QAction::toggled, this, [=](bool checked)
	{
        if (checked)
            menu->showByMainMenu(this);
		else
			menu->close();
	});

	connect(menu, &QCtmNavigationSidePanel::paneClosed, action, [=]() {
		action->setChecked(false);
	});

}

int QCtmNavigationMainMenu::indexOf(QAction* action) const
{
    auto item = ::find(action, m_impl->items);
	return m_impl->items.indexOf(item);
}

int QCtmNavigationMainMenu::count() const
{
	return m_impl->items.count();
}

QAction* QCtmNavigationMainMenu::actionAt(int index) const
{
	if (m_impl->items.size() <= index)
		return nullptr;
	return m_impl->items.at(index)->action();
}

void QCtmNavigationMainMenu::setItemSize(const QSize& size)
{
    m_impl->actionWidth = size.width();
    m_impl->actionHeight = size.height();
    for (auto item : m_impl->items)
    {
        if (!item->isCustomWidget())
        {
            item->widget()->setFixedSize(size);
        }
    }
}

QSize QCtmNavigationMainMenu::itemSize() const
{
    return QSize(m_impl->actionWidth, m_impl->actionHeight);
}

void QCtmNavigationMainMenu::setHorizontalSpacing(int spacing)
{
	m_impl->layout->setHorizontalSpacing(spacing);
	m_impl->layout->invalidate();
	m_impl->layout->update();
}

int QCtmNavigationMainMenu::horizontalSpacing() const
{
	return m_impl->layout->horizontalSpacing();
}

void QCtmNavigationMainMenu::setVerticalSpacing(int spacing)
{
	m_impl->layout->setVerticalSpacing(spacing);
	m_impl->layout->invalidate();
	m_impl->layout->update();
}

int QCtmNavigationMainMenu::verticalSpacing() const
{
	return m_impl->layout->verticalSpacing();
}

void QCtmNavigationMainMenu::setColorAlternate(bool enabled)
{
    m_impl->colorAlternate = enabled;
}

bool QCtmNavigationMainMenu::colorAlternate() const
{
    return m_impl->colorAlternate;
}

QAction* QCtmNavigationMainMenu::addGroupingMenu(const QIcon& icon, const QString& text, QCtmNavigationBackstageMenu * menu)
{
	auto action = addAction(icon, text);
	bindMenu(action, menu);
	return action;
}

QAction* QCtmNavigationMainMenu::addGroupingMenu(const QString& text, QCtmNavigationBackstageMenu * menu)
{
	return addGroupingMenu(QIcon(), text, menu);
}

bool QCtmNavigationMainMenu::event(QEvent * e)
{
	if (e->type() == QEvent::StyleChange)
	{
		this->metaObject()->invokeMethod(this, "relayout", Qt::QueuedConnection);
        this->metaObject()->invokeMethod(this, "calcItemColor", Qt::QueuedConnection);
	}
	return QCtmNavigationSidePanel::event(e);
}

void QCtmNavigationMainMenu::actionEvent(QActionEvent *event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        QCtmWidgetItemForMainMenuPtr item = std::make_shared<QCtmWidgetItemForMainMenu>(event->action(), Qt::Horizontal, this);
        if (!item->isCustomWidget())
        {
            auto btn = qobject_cast<QCtmMainMenuButton*>(item->widget());
            if (btn)
            {
                btn->setFixedSize(m_impl->actionWidth, m_impl->actionHeight);
                btn->setAlternateColor(QCtmColor::generalBackgroundColor(this->count()));
                btn->setAlternateEnable(m_impl->colorAlternate);
            }
        }
        Util::addItem(item, m_impl->items, event->before(), m_impl->layout);
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        auto item = ::find(event->action(), m_impl->items);
        m_impl->items.removeOne(item);
        m_impl->layout->takeAt(m_impl->layout->indexOf(item->widget()));
    }
}

QSize QCtmNavigationMainMenu::sizeHint() const
{
	auto count = m_impl->items.size();
	auto size = QCtmNavigationSidePanel::sizeHint();
	if (!count)
		return size;

	switch (dockArea())
	{
	case DockArea::Left:
	case DockArea::Right:
		size.setWidth(m_impl->layout->minimumSize().width() + 2);
		break;
	case DockArea::Top:
	case DockArea::Bottom:
	{
		auto hMax = smartSize(DockArea::Left).height();
		size.setHeight(m_impl->layout->heightForWidth(width()) + 2 > hMax ? hMax : m_impl->layout->heightForWidth(width()) + 11);
		break;
	}
	}
	return size;
}

void QCtmNavigationMainMenu::relayout()
{
	QStyleOption opt;
	opt.initFrom(this);
	auto rect = style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
	QMargins margin(rect.left(), rect.top(), this->width() - rect.left() - rect.width(), this->height() - rect.top() - rect.height());
	m_impl->layout->setContentsMargins(margin);
	m_impl->layout->invalidate();
	m_impl->layout->update();
}

void QCtmNavigationMainMenu::calcItemColor()
{
    if (!m_impl->colorAlternate)
        return;
    int i = 0;
    for (auto &&item : m_impl->items)
    {
        if (item->isCustomWidget())
            continue;
        auto btn = qobject_cast<QCtmMainMenuButton*>(item->widget());
        if (btn)
        {
            btn->setAlternateEnable(m_impl->colorAlternate);
        }
    }
}
