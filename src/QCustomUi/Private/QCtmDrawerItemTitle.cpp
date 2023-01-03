/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2023 LiLong                                              **
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
#include "QCtmDrawerItemTitle_p.h"
#include "QCtmDrawerItemWidget.h"
#include "QCtmWidgetItem_p.h"
#include "Util_p.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

struct QCtmDrawerItemTitle::Impl
{
    bool expand { false };
    bool pressed { false };
    QHBoxLayout* layout { nullptr };
    QHBoxLayout* widgetLayout { nullptr };
    QList<QCtmWidgetItemPtr> items;
    QCtmDrawerItemWidget* treeItem { nullptr };
    QSize iconSize { 16, 16 };
};

QCtmDrawerItemTitle::QCtmDrawerItemTitle(QCtmDrawerItemWidget* parent) : QPushButton(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->treeItem = parent;
    m_impl->layout   = new QHBoxLayout(this);
    m_impl->layout->setContentsMargins(0, 0, 0, 0);
    m_impl->layout->setSpacing(0);
    m_impl->layout->addStretch(1);
    m_impl->widgetLayout = new QHBoxLayout;
    m_impl->widgetLayout->setContentsMargins(0, 0, 0, 0);
    m_impl->widgetLayout->setSpacing(0);
    m_impl->layout->addLayout(m_impl->widgetLayout);
    m_impl->widgetLayout->setAlignment(Qt::AlignRight);
}

QCtmDrawerItemTitle::~QCtmDrawerItemTitle() {}

void QCtmDrawerItemTitle::setExpand(bool expand)
{
    m_impl->expand = expand;
    if (isVisible())
        update();
}

bool QCtmDrawerItemTitle::isExpand() const { return m_impl->expand; }

void QCtmDrawerItemTitle::insertAction(int index, QAction* action)
{
    auto before = actionAt(index);
    QWidget::insertAction(before, action);
}

QAction* QCtmDrawerItemTitle::actionAt(int index) const
{
    if (index < 0)
        return nullptr;
    if (m_impl->items.size() > index)
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

void QCtmDrawerItemTitle::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(m_impl->iconSize);
}

const QSize& QCtmDrawerItemTitle::iconSize() const { return m_impl->iconSize; }

void QCtmDrawerItemTitle::paintEvent([[maybe_unused]] QPaintEvent* event)
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
        itemOpt.rect             = QRect(0, 0, indent, height());
        itemOpt.viewItemPosition = QStyleOptionViewItem::Beginning;
        itemOpt.state            = QStyle::State_Children;
        if (m_impl->expand)
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

void QCtmDrawerItemTitle::actionEvent(QActionEvent* event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        QCtmWidgetItemPtr item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCtmDrawerItemTitle::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
        Util::addItem(item, m_impl->items, event->before(), m_impl->widgetLayout);
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        Util::removeItem(event->action(), m_impl->items, m_impl->widgetLayout);
    }
}
