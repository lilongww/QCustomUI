#include "QCtmToolBoxItem_p.h"
#include "QCtmWidgetItem_p.h"
#include "Util_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QActionEvent>
#include <QList>

struct QCtmToolBoxItem::Impl
{
    QList<QCtmWidgetItemPtr> items;
    QHBoxLayout* layout{ nullptr };
    QString title;
    QIcon icon;
    QWidget* m_content{ nullptr };
    int m_stretch{ 0 };
};

QCtmToolBoxItem::QCtmToolBoxItem(QWidget *parent)
	: QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
	ui.setupUi(this);
	setObjectName("item");
    m_impl->layout = new QHBoxLayout(ui.item_title);
    m_impl->layout->setAlignment(Qt::AlignRight);
    m_impl->layout->setMargin(0);
    m_impl->layout->setSpacing(0);
    ui.item_title->installEventFilter(this);
}

QCtmToolBoxItem::~QCtmToolBoxItem()
{
}

void QCtmToolBoxItem::setTitle(const QString& text)
{
    m_impl->title = text;
}

QString QCtmToolBoxItem::title() const
{
    return m_impl->title;
}

void QCtmToolBoxItem::setContent(QWidget* widget)
{
    if (m_impl->m_content)
    {
        for (auto action : m_impl->m_content->actions())
        {
            removeAction(action);
        }
        delete m_impl->m_content;
    }
	ui.contentLayout->addWidget(widget);
    m_impl->m_content = widget;
    for (auto action : m_impl->m_content->actions())
    {
        addAction(action);
    }
    m_impl->m_content->installEventFilter(this);
}

QWidget* QCtmToolBoxItem::content() const
{
	return m_impl->m_content;
}

void QCtmToolBoxItem::setStretch(int stretch)
{
    m_impl->m_stretch = stretch;
}

int QCtmToolBoxItem::stretch() const
{
	return m_impl->m_stretch;
}

void QCtmToolBoxItem::setIcon(const QIcon& icon)
{
    m_impl->icon = icon;
}

const QIcon& QCtmToolBoxItem::icon() const
{
    return m_impl->icon;
}

void QCtmToolBoxItem::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCtmToolBoxItem::actionEvent(QActionEvent *event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, this);
        Util::addItem(item, m_impl->items, event->before(), m_impl->layout);
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->items, m_impl->layout);
    }
}

bool QCtmToolBoxItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_impl->m_content)
    {
        if (event->type() == QEvent::ActionAdded)
        {
            auto evt = dynamic_cast<QActionEvent*>(event);
            if (evt)
                insertAction(evt->before(), evt->action());
        }
        else if (event->type() == QEvent::ActionRemoved)
        {
            auto evt = dynamic_cast<QActionEvent*>(event);
            if (evt)
            {
                removeAction(evt->action());
            }
        }
    }
    else if (watched == ui.item_title)
    {
        if (event->type() == QEvent::Paint)
        {
            QPainter p(ui.item_title);
            QStyleOptionViewItem opt;
            opt.initFrom(ui.item_title);
            opt.text = m_impl->title;
            opt.icon = m_impl->icon;
            opt.font = ui.item_title->font();
            opt.decorationAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            opt.features = QStyleOptionViewItem::HasDisplay;
            opt.rect = ui.item_title->style()->subElementRect(QStyle::SE_FrameContents, &opt, ui.item_title);
            if (!opt.icon.isNull())
                opt.features |= QStyleOptionViewItem::HasDecoration;
            ui.item_title->style()->drawControl(QStyle::CE_ItemViewItem, &opt, &p, ui.item_title);
            return true;
        }
    }
    return false;
}
