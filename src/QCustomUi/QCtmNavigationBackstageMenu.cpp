#include "QCtmNavigationBackstageMenu.h"
#include "QCtmNavigationBackstageActionGroup.h"
#include "QCtmNavigationMainMenu.h"
#include "Private/Util_p.h"

#include <QHash>
#include <QActionGroup>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>

#include <assert.h>

struct QCtmNavigationBackstageMenu ::Impl
{
	QList<QCtmNavigationBackstageActionGroup*> subMenus;
	QHBoxLayout* layout{ nullptr };
	QCtmNavigationMainMenu* mainMenu{ nullptr };
	QWidget* viewPort{ nullptr };
    QPoint closePos;
    QColor baseColor;

	bool contains(const QString& groupName)const
	{
		for (auto menu : subMenus)
		{
			if (menu->groupName() == groupName)
				return true;
		}
		return false;
	}

	QCtmNavigationBackstageActionGroup* subMenu(const QString& groupName)const
	{
		for (auto menu : subMenus)
		{
			if (menu->groupName() == groupName)
				return menu;
		}
		return nullptr;
	}

	void remove(const QString& groupName)
	{
		auto menu = subMenu(groupName);
		if (menu)
			subMenus.removeOne(menu);
	}

	int indexOf(const QString& groupName)
	{
		int index = 0;
		for (auto menu : subMenus)
		{
			if (menu->groupName() == groupName)
				return index;
			index++;
		}
		return -1;
	}
};

QCtmNavigationBackstageMenu ::QCtmNavigationBackstageMenu (QCtmNavigationBar *parent)
	: QCtmNavigationSidePanel(parent)
	, m_impl(std::make_shared<Impl>())
{
	m_impl->viewPort = new QWidget(this);
	m_impl->viewPort->setObjectName("viewport");
	auto layout = new QHBoxLayout(m_impl->viewPort);
	layout->setMargin(20);
	m_impl->layout = new QHBoxLayout();
	m_impl->layout->setMargin(0);
	layout->addLayout(m_impl->layout);
	layout->addStretch(1);
	setWidget(m_impl->viewPort);
	m_impl->viewPort->setAutoFillBackground(false);
	setDockArea(DockArea::Top);
	resize(width(), 500);
	setMinimumHeight(0);
}

QCtmNavigationBackstageMenu ::~QCtmNavigationBackstageMenu ()
{
}

bool QCtmNavigationBackstageMenu ::addSubMenu(QCtmNavigationBackstageActionGroup* menu)
{
	return insertSubMenu(m_impl->subMenus.count(), menu);
}

QCtmNavigationBackstageActionGroup* QCtmNavigationBackstageMenu ::addSubMenu(const QString& groupName)
{
	if (m_impl->contains(groupName))
		return m_impl->subMenu(groupName);
	else
	{
		auto subMenu = new QCtmNavigationBackstageActionGroup(groupName, this);
		addSubMenu(subMenu);
		return subMenu;
	}
}

void QCtmNavigationBackstageMenu ::removeSubMenu(const QString& groupName)
{
	if (m_impl->contains(groupName))
	{
		auto menu = m_impl->subMenu(groupName);
		delete menu;
		m_impl->remove(groupName);
	}
}

void QCtmNavigationBackstageMenu ::removeSubMenu(QCtmNavigationBackstageActionGroup* menu)
{
	removeSubMenu(menu->groupName());
}

bool QCtmNavigationBackstageMenu ::insertSubMenu(int index, QCtmNavigationBackstageActionGroup* menu)
{
	if (m_impl->contains(menu->groupName()))
	{
		if (m_impl->subMenu(menu->groupName()) == menu)
		{
			auto oldIndex = m_impl->indexOf(menu->groupName());
			m_impl->remove(menu->groupName());
			m_impl->subMenus.insert(index >= oldIndex ? index : index - 1, menu);
			m_impl->layout->insertWidget(index, menu);
			return true;
		}
		return false;
	}
	m_impl->subMenus.insert(index, menu);
	m_impl->layout->insertWidget(index, menu);
	return true;
}

QCtmNavigationBackstageActionGroup* QCtmNavigationBackstageMenu ::insertSubMenu(int index, const QString& groupName)
{
	if (m_impl->contains(groupName))
	{
		auto menu = m_impl->subMenu(groupName);
		auto oldIndex = m_impl->indexOf(groupName);
		m_impl->remove(groupName);
		m_impl->subMenus.insert(index >= oldIndex ? index : index - 1, menu);
		return menu;
	}
	QCtmNavigationBackstageActionGroup* menu = new QCtmNavigationBackstageActionGroup(groupName, this);
	insertSubMenu(index, menu);
	return menu;
}

int QCtmNavigationBackstageMenu ::indexOf(const QString& groupName)
{
	return m_impl->indexOf(groupName);
}

int QCtmNavigationBackstageMenu ::count() const
{
	return m_impl->subMenus.size();
}

void QCtmNavigationBackstageMenu ::showByMainMenu(QCtmNavigationMainMenu* mainMenu)
{
	assert(mainMenu);
	m_impl->mainMenu = mainMenu;
	m_impl->mainMenu->installEventFilter(this);
	show();
}

QCtmNavigationBackstageActionGroup* QCtmNavigationBackstageMenu ::subMenuAt(int index) const
{
	if (m_impl->subMenus.size() <= index)
		return nullptr;
	return m_impl->subMenus.at(index);
}

void QCtmNavigationBackstageMenu ::closeEvent(QCloseEvent *event)
{
	if (m_impl->mainMenu)
	{
        if (windowFlags().testFlag(Qt::Popup))
        {
            if (m_impl->mainMenu->windowFlags().testFlag(Qt::Popup))
            {
                if (!m_impl->mainMenu->rect().contains(m_impl->mainMenu->mapFromGlobal(m_impl->closePos)))
                    m_impl->mainMenu->close();
            }
        }
		m_impl->mainMenu->removeEventFilter(this);
		m_impl->mainMenu = nullptr;
	}
	QCtmNavigationSidePanel::closeEvent(event);
}

QSize QCtmNavigationBackstageMenu ::sizeHint() const
{
	auto size = QCtmNavigationSidePanel::sizeHint();
	auto margin = m_impl->viewPort->layout()->margin();
	int left, top, right, bottom;
	this->layout()->getContentsMargins(&left, &top, &right, &bottom);
	auto h = m_impl->layout->sizeHint().height() + 2 + margin * 2;
	auto maxH = smartSize(DockArea::Left).height();
	if (m_impl->mainMenu)
	{
		maxH -= m_impl->mainMenu->height();
	}
	auto maxHeight = maxH;
	size.setHeight(h > maxHeight ? maxHeight : h + this->layout()->contentsMargins().bottom());
	if (viewContainer()->horizontalScrollBar()->isVisible())
	{
		size.setHeight(size.height() + viewContainer()->horizontalScrollBar()->height() + top + bottom + 2);
	}
	return size;
}

QPoint QCtmNavigationBackstageMenu ::smartPosition(DockArea area) const
{
	auto pos = QCtmNavigationSidePanel::smartPosition(area);
	if (area == DockArea::Top)
	{
		if (m_impl->mainMenu)
			pos.setY(m_impl->mainMenu->height() + pos.y() - m_impl->mainMenu->layout()->contentsMargins().bottom());
		return pos;
	}
	return pos;
}

bool QCtmNavigationBackstageMenu ::eventFilter(QObject* o, QEvent* e)
{
	if (o == m_impl->mainMenu)
	{
		switch (e->type())
		{
		case QEvent::Resize:
			resize(sizeHint());
			move(smartPosition(dockArea()));
			break;
		case QEvent::Close:
		case QEvent::Hide:
			this->close();
			break;
		}
	}
	return QCtmNavigationSidePanel::eventFilter(o, e);
}

void QCtmNavigationBackstageMenu::setBaseColor(const QColor& color)
{
    m_impl->baseColor = color;
}

const QColor& QCtmNavigationBackstageMenu::baseColor() const
{
    return m_impl->baseColor;
}

void QCtmNavigationBackstageMenu::mousePressEvent(QMouseEvent *event)
{
    if (windowFlags().testFlag(Qt::Popup))
    {
        m_impl->closePos = mapToGlobal(event->pos());
    }
    QCtmNavigationSidePanel::mousePressEvent(event);
}

void QCtmNavigationBackstageMenu::paintEvent(QPaintEvent *event)
{
    QCtmNavigationSidePanel::paintEvent(event);
    if (m_impl->mainMenu)
    {
        QPainter p(this);
        p.setPen(m_impl->baseColor);
        p.drawLine(0, 0, width(), 0);
    }
}

