/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
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
#include "QCtmTableViewButtonsDelegate.h"
#include "QCtmColor.h"
#include "QCtmTableView.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QTableView>

#include <algorithm>
#include <map>
#include <vector>

struct QCtmTableViewButtonsDelegate::Impl
{
    struct ButtonData
    {
        QString text;
        QBrush normal;
        QBrush hover;
        QBrush pressed;
        QColor textColor;
        int borderRadius { 0 };
        QSize size;

        const QBrush& getBrush(bool isHover, bool isPressed) const
        {
            if (isPressed)
                return pressed;
            if (isHover)
                return hover;
            return normal;
        }
        QPointF mapFromView(const QRect& rect, const QPointF& p) const
        {
            return QPointF(p.x() - rect.x(), rect.y() - p.y());
        }
    };
    QCtmTableView* view;
    bool uniformSize { true };
    std::vector<ButtonData> buttons;
    int space { 5 };
    int pressedBtn { -1 };
    std::optional<QPoint> hoverPoint;
    std::optional<QPoint> pressedPoint;
    int column { -1 };
    Qt::Alignment alignment { Qt::AlignCenter };
    inline Impl(QCtmTableView* v) : view(v)
    {
    }

    inline int totalWidth() const
    {
        auto width = std::accumulate(buttons.begin(),
                                     buttons.end(),
                                     0,
                                     [](int a, const auto& btn)
                                     {
                                         return a + btn.size.width();
                                     });
        return width + static_cast<int>(space * (buttons.size() - 1));
    }
    inline std::vector<QRect> calcRects(const QStyleOptionViewItem& option) const
    {
        auto rect = option.widget->style()->subElementRect(QStyle::SE_HeaderLabel, &option, option.widget);
        if (buttons.empty())
            return {};
        int x = 0;
        switch (alignment)
        {
        case Qt::AlignLeft:
            x = rect.x();
            break;
        case Qt::AlignRight:
            x = rect.x() + rect.width() - totalWidth();
            break;
        default:
            x = (rect.width() - totalWidth()) / 2 + rect.x(); // 默认为中央对齐
        }
        auto y = rect.y() + (rect.height() - buttons.front().size.height()) / 2;
        std::vector<QRect> rects;
        for (int i = 0; i < buttons.size(); ++i)
        {
            rects.push_back(QRect(QPoint(x, y), buttons[i].size));
            x += buttons[i].size.width() + space;
        }
        return rects;
    }

    inline int buttonAt(const QPoint& pos, const std::vector<QRect>& rects) const
    {
        auto it = std::find_if(rects.begin(),
                               rects.end(),
                               [&](const auto& rect)
                               {
                                   return rect.contains(pos);
                               });
        return static_cast<int>(it == rects.end() ? -1 : std::distance(rects.begin(), it));
    }

    inline void calcSizes()
    {
        auto fm = view->fontMetrics();
        QSize maxSize;
        QStyleOption opt;
        opt.initFrom(view);
        int margin = view->style()->pixelMetric(QStyle::PM_ButtonMargin, &opt, view);
        for (auto& btn : buttons)
        {
            btn.size = btn.size.expandedTo(fm.boundingRect(btn.text).marginsAdded(QMargins(margin, margin, margin, margin)).size());
            if (uniformSize)
            {
                maxSize = maxSize.expandedTo(btn.size);
            }
        }
        if (uniformSize)
        {
            for (auto& btn : buttons)
            {
                btn.size = maxSize;
            }
        }
    }

    inline void clearHover()
    {
        hoverPoint = std::nullopt;
    }

    inline QMouseEvent makeMouseEvent(ButtonData& btn, const QRect& rect, QMouseEvent* e)
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        return QMouseEvent(e->type(), btn.mapFromView(rect, e->pos()), e->globalPos(), e->button(), e->buttons(), e->modifiers());
#else
        return QMouseEvent(e->type(), btn.mapFromView(rect, e->position()), e->globalPosition(), e->button(), e->buttons(), e->modifiers());
#endif
    }
};

/*!
    \class      QCtmTableViewButtonsDelegate
    \brief      TableView按钮委托类.
    \inherits   QCtmTableItemDelegate
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmTableViewButtonsDelegate.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmTableViewButtonsDelegate::QCtmTableViewButtonsDelegate(QCtmTableView* parent)
    : QCtmTableItemDelegate(parent), m_impl(std::make_unique<Impl>(parent))
{
    parent->viewport()->setMouseTracking(true);
    parent->viewport()->installEventFilter(this);
}

/*!
    \brief      析构函数.
*/
QCtmTableViewButtonsDelegate::~QCtmTableViewButtonsDelegate()
{
}

/*!
    \reimp
*/
QSize QCtmTableViewButtonsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto size = QCtmTableItemDelegate::sizeHint(option, index);
    return size.expandedTo(QSize(m_impl->totalWidth(), size.height()));
}

/*!
    \brief      添加一个按钮，文本为 \a text, 背景色为 \a background, 悬停时背景色为 \a hover, 按下时背景色为 \a pressed, 文本颜色为 \a
                textColor.
*/
void QCtmTableViewButtonsDelegate::addButton(const QString& text,
                                             const QBrush& background,
                                             const QBrush& hover,
                                             const QBrush& pressed,
                                             const QColor& textColor)
{
    m_impl->buttons.push_back(
        Impl::ButtonData { .text      = text,
                           .normal    = background,
                           .hover     = hover,
                           .pressed   = pressed == QBrush(Qt::NoBrush) ? hover : pressed,
                           .textColor = textColor.isValid() ? textColor : QCtmColor::generateForegroundColor(background.color()) });
    m_impl->calcSizes();
    update();
}

/*!
    \brief      设置按钮的大小是否统一，\a on 为 true 时表示统一大小，默认为 true.
    \sa         uniformButtonSize
*/
void QCtmTableViewButtonsDelegate::setUniformButtonSize(bool on)
{
    m_impl->uniformSize = on;
    m_impl->calcSizes();
    update();
}

/*!
    \brief      返回按钮的大小是否统一，默认为 true.
    \sa         setUniformButtonSize
*/
bool QCtmTableViewButtonsDelegate::uniformButtonSize() const
{
    return m_impl->uniformSize;
}

/*!
    \brief      设置按钮 \a button 的大小为 \a size, 如果 uniformButtonSize() 为 true，则所有按钮的大小都会被设置为 \a size.
    \sa         buttonSize
*/
void QCtmTableViewButtonsDelegate::setButtonSize(int button, const QSize& size)
{
    if (button < 0 || button >= m_impl->buttons.size())
        return;
    if (m_impl->uniformSize)
    {
        for (int i = 0; i < m_impl->buttons.size(); ++i)
        {
            m_impl->buttons[i].size = size;
        }
    }
    else
        m_impl->buttons[button].size = size;
    m_impl->calcSizes();
    update();
}

/*!
    \brief      返回按钮 \a button 的大小.
    \sa         setButtonSize
*/
const QSize& QCtmTableViewButtonsDelegate::buttonSize(int button) const
{
    static QSize emptySize;
    if (button < 0 || button >= m_impl->buttons.size())
        return emptySize;
    return m_impl->buttons[button].size;
}

/*!
    \brief      设置按钮之间的间距为 \a space, 默认为 5.
    \sa         space
*/
void QCtmTableViewButtonsDelegate::setSpace(int space)
{
    m_impl->space = space;
    update();
}

/*!
    \brief      返回按钮之间的间距，默认为 5.
    \sa         setSpace
*/
int QCtmTableViewButtonsDelegate::space() const
{
    return m_impl->space;
}

/*!
    \brief      设置水平对齐方式为 \a alignment, 默认为 Qt::AlignCenter.
    \sa         alignment
*/
void QCtmTableViewButtonsDelegate::setAlignment(Qt::Alignment alignment)
{
    m_impl->alignment = alignment;
    update();
}

/*!
    \brief      返回水平对齐方式，默认为 Qt::AlignCenter.
    \sa         setAlignment
*/
Qt::Alignment QCtmTableViewButtonsDelegate::alignment() const
{
    return m_impl->alignment;
}

/*!
    \brief      刷新视图，触发重绘.
*/
void QCtmTableViewButtonsDelegate::update()
{
    m_impl->view->viewport()->update();
}

/*!
    \reimp
*/
void QCtmTableViewButtonsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QCtmTableItemDelegate::paint(painter, option, index);
    drawButtons(painter, option, index);
}

/*!
    \brief      绘制按钮，使用 \a painter 绘制，使用 \a option 作为样式选项，使用 \a index 作为模型索引.
*/
void QCtmTableViewButtonsDelegate::drawButtons(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto rects = m_impl->calcRects(option);
    for (int i = 0; i < m_impl->buttons.size(); ++i)
    {
        const auto& btn = m_impl->buttons[i];
        const auto& r   = rects[i];
        bool isHover    = m_impl->hoverPoint ? r.contains(*m_impl->hoverPoint) : false;
        bool isPressed  = m_impl->pressedPoint ? r.contains(*m_impl->pressedPoint) : false;
        painter->fillRect(r, btn.getBrush(isHover, isPressed));
        painter->setPen(QPen(btn.textColor));
        painter->drawText(r, btn.text, QTextOption(Qt::AlignCenter));
    }
}

/*!
    \reimp
*/
bool QCtmTableViewButtonsDelegate::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_impl->view->viewport())
    {
        auto createOption = [this](const QModelIndex& index)
        {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QStyleOptionViewItem option = m_impl->view->viewOptions();
#else
            QStyleOptionViewItem option;
            m_impl->view->initViewItemOption(&option);
#endif
            option.rect = m_impl->view->visualRect(index);
            this->initStyleOption(&option, index);
            return option;
        };
        switch (event->type())
        {
        case QEvent::MouseMove:
            {
                auto e     = dynamic_cast<QMouseEvent*>(event);
                auto p     = e->pos();
                auto index = m_impl->view->indexAt(p);
                if (!index.isValid() || index.column() != m_impl->column)
                    break;
                m_impl->hoverPoint = p;
                auto rects         = m_impl->calcRects(createOption(index));
                auto btn           = m_impl->buttonAt(p, rects);
                if (btn == -1)
                    break;
                auto transEvent = m_impl->makeMouseEvent(m_impl->buttons[btn], rects[btn], e);
                emit m_impl->view->model()->dataChanged(index, index);
                mouseMoveEvent(btn, index, &transEvent);
            }
            break;
        case QEvent::MouseButtonPress:
            {
                auto e     = dynamic_cast<QMouseEvent*>(event);
                auto p     = e->pos();
                auto index = m_impl->view->indexAt(p);
                if (!index.isValid() || index.column() != m_impl->column)
                    break;
                m_impl->pressedPoint = p;
                auto rects           = m_impl->calcRects(createOption(index));
                auto btn             = m_impl->buttonAt(p, rects);
                if (btn == -1)
                    break;
                auto transEvent = m_impl->makeMouseEvent(m_impl->buttons[btn], rects[btn], e);
                emit m_impl->view->model()->dataChanged(index, index);
                mousePressEvent(btn, index, &transEvent);
            }
            break;
        case QEvent::MouseButtonRelease:
            {
                if (!m_impl->pressedPoint)
                    break;
                auto e     = dynamic_cast<QMouseEvent*>(event);
                auto index = m_impl->view->indexAt(*m_impl->pressedPoint);
                if (!index.isValid() || index.column() != m_impl->column)
                    break;
                auto rects           = m_impl->calcRects(createOption(index));
                auto btn             = m_impl->buttonAt(e->pos(), rects);
                m_impl->pressedPoint = std::nullopt;
                if (btn != -1)
                {
                    auto e          = dynamic_cast<QMouseEvent*>(event);
                    auto transEvent = m_impl->makeMouseEvent(m_impl->buttons[btn], rects[btn], e);
                    auto index      = m_impl->view->indexAt(e->pos());
                    emit m_impl->view->model()->dataChanged(index, index);
                    mouseReleaseEvent(btn, index, &transEvent);
                }
            }
            break;
        case QEvent::Leave:
            m_impl->clearHover();
            update();
            break;
        default:
            break;
        }
    }
    return QCtmTableItemDelegate::eventFilter(object, event);
}

/*!
    \reimp
*/
void QCtmTableViewButtonsDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QCtmTableItemDelegate::initStyleOption(option, index);
    m_impl->column = index.column();
}

/*!
    \brief      鼠标移动事件处理，\a button 为按钮索引，\a index 为模型索引，\a e 为鼠标事件.
*/
void QCtmTableViewButtonsDelegate::mouseMoveEvent(int button, const QModelIndex& index, QMouseEvent* e)
{
}

/*!
    \brief      鼠标按下事件处理，\a button 为按钮索引，\a index 为模型索引，\a e 为鼠标事件.
*/
void QCtmTableViewButtonsDelegate::mousePressEvent(int button, const QModelIndex& index, QMouseEvent* e)
{
    m_impl->pressedBtn = button;
}

/*!
    \brief      鼠标释放事件处理，\a button 为按钮索引，\a index 为模型索引，\a e 为鼠标事件.
*/
void QCtmTableViewButtonsDelegate::mouseReleaseEvent(int button, const QModelIndex& index, QMouseEvent* e)
{
    if (m_impl->pressedBtn == button)
    {
        emit buttonClicked(button, index);
    }
    m_impl->pressedBtn = -1;
}
