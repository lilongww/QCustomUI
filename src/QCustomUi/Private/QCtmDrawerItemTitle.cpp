#include "QCtmDrawerItemTitle_p.h"
#include "Util_p.h"
#include "QCtmWidgetItem_p.h"
#include "QCtmDrawerItemWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QHBoxLayout>

struct QCtmDrawerItemTitle::Impl
{
	bool expand{ false };
    bool pressed{ false };
    QHBoxLayout* layout{ nullptr };
    QHBoxLayout* widgetLayout{ nullptr };
    QList<QCtmWidgetItemPtr> items;
    QCtmDrawerItemWidget* treeItem{ nullptr };
};

QCtmDrawerItemTitle::QCtmDrawerItemTitle(QCtmDrawerItemWidget *parent)
	: QPushButton(parent)
	, m_impl(std::make_unique<Impl>())
{
    m_impl->treeItem = parent;
    m_impl->layout = new QHBoxLayout(this);
    m_impl->layout->setMargin(0);
    m_impl->layout->setSpacing(0);
    m_impl->layout->addStretch(1);
    m_impl->widgetLayout = new QHBoxLayout;
    m_impl->widgetLayout->setMargin(0);
    m_impl->widgetLayout->setSpacing(0);
    m_impl->layout->addLayout(m_impl->widgetLayout);
    m_impl->widgetLayout->setAlignment(Qt::AlignRight);
}

QCtmDrawerItemTitle::~QCtmDrawerItemTitle()
{
}

void QCtmDrawerItemTitle::setExpand(bool expand)
{
	m_impl->expand = expand;
	if (isVisible())
		update();
}

bool QCtmDrawerItemTitle::isExpand() const
{
	return m_impl->expand;
}

void QCtmDrawerItemTitle::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

QAction* QCtmDrawerItemTitle::actionAt(int index) const
{
    if (index < 0)
        return nullptr;
    if (m_impl->items.size()>index)
    {
        return m_impl->items.at(index)->action();
    }
    return nullptr;
}

int QCtmDrawerItemTitle::indexOf(QAction* action) const
{
    auto item = Util::find(action, m_impl->items);
    return m_impl->items.indexOf(item);
}

void QCtmDrawerItemTitle::paintEvent([[maybe_unused]] QPaintEvent *event)
{
	QPainter p(this);
	QStyleOptionButton opt;
    this->initStyleOption(&opt);
    opt.text.clear();
    style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);

	QStyleOptionViewItem itemOpt;
	itemOpt.initFrom(this);
	auto indent = style()->pixelMetric(QStyle::PM_TreeViewIndentation, &itemOpt, this);
    do
	{
        if (!m_impl->treeItem->widget())
            break;
		itemOpt.rect = QRect(0, 0, indent, height());
		itemOpt.viewItemPosition = QStyleOptionViewItem::Beginning;
        itemOpt.state = QStyle::State_Children;
		if(m_impl->expand)
			itemOpt.state |= QStyle::State_Open;
		style()->drawPrimitive(QStyle::PE_IndicatorBranch, &itemOpt, &p, this);
    } while (false);
	{
		auto rect = QRect(opt.rect.left() + indent, opt.rect.top(), opt.rect.width() - 40, opt.rect.height());
		QTextOption to;
		to.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		p.save();
		p.setPen(opt.palette.windowText().color());
		p.drawText(rect, text(), to);
		p.restore();
	}
}

void QCtmDrawerItemTitle::actionEvent(QActionEvent *event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        QCtmWidgetItemPtr item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
        Util::addItem(item, m_impl->items, event->before(), m_impl->widgetLayout);
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->items, m_impl->widgetLayout);
    }
}
