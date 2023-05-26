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
#include "QCtmRecentViewDelegate.h"
#include "Private/QCtmXPM.h"
#include "QCtmRecentModel.h"
#include "QCtmRecentView.h"

#include <QApplication>
#include <QFontMetricsF>
#include <QMouseEvent>
#include <QPainter>
#include <QToolButton>

constexpr static int SpacePixel = 15;

/*!
    \class      QCtmRecentViewDelegate
    \brief      仿vs启动界面最近项目表委托.
    \inherits   QStyledItemDelegate
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmRecentViewDelegate.h
*/

struct QCtmRecentViewDelegate::Impl
{
    std::optional<QPoint> mousePoint;
    bool pressed { false };
    std::optional<QPersistentModelIndex> pressedIndex;
    bool topButtonVisible { true };
    QToolButton* topButton;
    inline Impl(QCtmRecentView* parent) : topButton(new QToolButton(parent))
    {
        QIcon icon;
        icon.addPixmap(QPixmap(QCtmXPM::UnFixedXPM));
        icon.addPixmap(QPixmap(QCtmXPM::FixedXPM), QIcon::Mode::Normal, QIcon::State::On);
        topButton->setIcon(icon);
        topButton->setCheckable(true);
        topButton->setIconSize({ 16, 16 });
        topButton->setAutoRaise(true);
        topButton->setVisible(false);
        topButton->setObjectName("top_button");
    }
};

/*!
    \brief      构造函数 \a parent.
*/
QCtmRecentViewDelegate::QCtmRecentViewDelegate(QCtmRecentView* parent /*= nullptr*/)
    : QStyledItemDelegate(parent), m_impl(std::make_unique<Impl>(parent))
{
}

/*!
    \brief      析构函数.
*/
QCtmRecentViewDelegate::~QCtmRecentViewDelegate() {}

/*!
    \brief      设置指定按钮图标 \a icon.
    \sa         topButtonIcon
*/
void QCtmRecentViewDelegate::setTopButtonIcon(const QIcon& icon) { m_impl->topButton->setIcon(icon); }

/*!
    \brief      返回置顶按钮图标.
    \sa         setTopButtonIcon
*/
QIcon QCtmRecentViewDelegate::topButtonIcon() const { return m_impl->topButton->icon(); }

/*!
    \brief      设置置顶按钮是否可见 \a v.
    \sa         topButtonVisible
*/
void QCtmRecentViewDelegate::setTopButtonVisible(bool v) { m_impl->topButtonVisible = v; }

/*!
    \brief      返回置顶按钮是否可见.
    \sa         setTopButtonVisible
*/
bool QCtmRecentViewDelegate::topButtonVisible() const { return m_impl->topButtonVisible; }

/*!
    \reimp
*/
void QCtmRecentViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.parent().isValid())
        QStyledItemDelegate::paint(painter, option, index);
    else
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        const QWidget* widget = option.widget;
        QStyle* style         = widget ? widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);
        drawIcon(painter, option, index);
        drawName(painter, option, index);
        drawPath(painter, option, index);
        drawTime(painter, option, index);
        if (m_impl->topButtonVisible)
            drawTopButton(painter, option, index);
    }
}

/*!
    \reimp
*/
QSize QCtmRecentViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto size = QStyledItemDelegate::sizeHint(option, index);
    if (!index.parent().isValid())
        return size;
    else
    {
        auto height = SpacePixel * 3 + option.fontMetrics.height() * 2;
        return size.expandedTo(QSize(size.width(), height));
    }
}

/*!
    \brief      绘制图标 \a parent, \a option, \a index.
*/
void QCtmRecentViewDelegate::drawIcon(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto icon = index.data(QCtmRecentModel::Roles::Icon).value<QIcon>();
    QIcon::Mode mode;
    if (option.widget->isEnabled())
    {
        mode = option.state.testFlag(QStyle::State_Selected) ? QIcon::Mode::Selected : QIcon::Mode::Normal;
    }
    else
        mode = QIcon::Mode::Disabled;
    auto rect = doIconRect(option);
    painter->drawPixmap(rect, icon.pixmap(rect.size(), mode, QIcon::State::Off));
}

/*!
    \brief      绘制名称 \a parent, \a option, \a index.
*/
void QCtmRecentViewDelegate::drawName(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->drawText(
        doNameRect(option), index.data(QCtmRecentModel::Roles::Name).toString(), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
}

/*!
    \brief      绘制路径 \a parent, \a option, \a index.
*/
void QCtmRecentViewDelegate::drawPath(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->drawText(
        doPathRect(option), index.data(QCtmRecentModel::Roles::Path).toString(), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
}

/*!
    \brief      绘制时间 \a parent, \a option, \a index.
*/
void QCtmRecentViewDelegate::drawTime(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->drawText(doTimeRect(option),
                      index.data(QCtmRecentModel::Roles::Time).toDateTime().toString("yyyy/MM/dd hh:mm"),
                      QTextOption(Qt::AlignRight | Qt::AlignVCenter));
}

/*!
    \brief      绘制置顶按钮 \a parent, \a option, \a index.
*/
void QCtmRecentViewDelegate::drawTopButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto fixed   = index.data(QCtmRecentModel::Roles::isTop).toBool();
    auto btnRect = doTopButtonRect(option);
    if (!fixed && (!m_impl->mousePoint || !option.rect.contains(*m_impl->mousePoint)))
        return;
    bool mouseOver = m_impl->mousePoint && btnRect.contains(*m_impl->mousePoint);
    m_impl->topButton->setAttribute(Qt::WA_UnderMouse, mouseOver && !m_impl->pressed);
    m_impl->topButton->setChecked(fixed);
    m_impl->topButton->resize(btnRect.size());
    auto p = btnRect.topLeft();
    painter->save();
    painter->setTransform(QTransform::fromTranslate(btnRect.left(), btnRect.top()));
    m_impl->topButton->render(painter);
    painter->restore();
}

/*!
    \brief      计算图标位置 \a option.
*/
QRect QCtmRecentViewDelegate::doIconRect(const QStyleOptionViewItem& option) const
{
    const auto& r = option.rect;
    return QRect(r.left() + SpacePixel, r.top() + SpacePixel, 24, 24);
}

/*!
    \brief      计算名称位置 \a option.
*/
QRect QCtmRecentViewDelegate::doNameRect(const QStyleOptionViewItem& option) const
{
    const auto& r = option.rect;
    auto iconRect = doIconRect(option);
    auto timeRect = doTimeRect(option);
    return QRect(
        iconRect.right() + SpacePixel, iconRect.top(), timeRect.left() - iconRect.right() - SpacePixel, option.fontMetrics.height());
}

/*!
    \brief      计算路径位置 \a option.
*/
QRect QCtmRecentViewDelegate::doPathRect(const QStyleOptionViewItem& option) const
{
    auto nameRect = doNameRect(option);
    return QRect(nameRect.left(), nameRect.bottom() + SpacePixel, option.rect.right() - nameRect.left(), option.fontMetrics.height());
}

/*!
    \brief      计算时间位置 \a option.
*/
QRect QCtmRecentViewDelegate::doTimeRect(const QStyleOptionViewItem& option) const
{
    auto w = option.fontMetrics.horizontalAdvance("2023/00/00 00:00");
    auto r = doTopButtonRect(option);
    return QRect(r.left() - SpacePixel - w, r.top(), w, option.fontMetrics.height());
}

/*!
    \brief      计算置顶按钮位置 \a option.
*/
QRect QCtmRecentViewDelegate::doTopButtonRect(const QStyleOptionViewItem& option) const
{
    const auto& r = option.rect;
    return QRect { r.right() - SpacePixel - 16, r.top() + SpacePixel, 16, 16 };
}

/*!
    \reimp
*/
bool QCtmRecentViewDelegate::eventFilter(QObject* object, QEvent* event)
{
    if (object->property("_interal_name").toString() == "qcustomui_qctmrecentview_viewport")
    {
        auto w = qobject_cast<QWidget*>(object);
        switch (event->type())
        {
        case QEvent::MouseMove:
            {
                auto e             = dynamic_cast<QMouseEvent*>(event);
                m_impl->mousePoint = e->position().toPoint();
                w->update();
            }
            break;
        case QEvent::MouseButtonPress:
            {
                auto e = dynamic_cast<QMouseEvent*>(event);
                if (e->button() == Qt::LeftButton)
                    m_impl->pressed = true;
                w->update();
            }
            break;
        case QEvent::MouseButtonRelease:
            {
                auto e = dynamic_cast<QMouseEvent*>(event);
                if (e->button() == Qt::LeftButton)
                    m_impl->pressed = false;
                w->update();
            }
            break;
        case QEvent::Leave:
            m_impl->mousePoint = std::nullopt;
            break;
        case QEvent::Enter:
            m_impl->mousePoint = w->mapFromGlobal(QCursor::pos());
            break;
        }
    }
    return QStyledItemDelegate::eventFilter(object, event);
}

/*!
    \reimp
*/
bool QCtmRecentViewDelegate::editorEvent(QEvent* event,
                                         QAbstractItemModel* model,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index)
{
    if (index.isValid())
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
            {
                auto e = dynamic_cast<QMouseEvent*>(event);
                m_impl->pressedIndex =
                    doTopButtonRect(option).contains(e->position().toPoint()) ? std::optional<QModelIndex>(index) : std::nullopt;
            }
            break;
        case QEvent::MouseButtonRelease:
            {
                auto e = dynamic_cast<QMouseEvent*>(event);
                if (m_impl->pressedIndex && *m_impl->pressedIndex == index && doTopButtonRect(option).contains(e->position().toPoint()))
                {
                    if (m_impl->topButtonVisible)
                        emit topButtonClicked(index);
                }
                m_impl->pressedIndex = std::nullopt;
            }
            break;
        }
    }
    return false;
}
