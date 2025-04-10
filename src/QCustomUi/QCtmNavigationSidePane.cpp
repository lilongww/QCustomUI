﻿/*********************************************************************************
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

#include "QCtmNavigationSidePane.h"
#include "QCtmNavigationBar.h"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QTimer>

#include <assert.h>

constexpr int margin = 8;

struct QCtmNavigationSidePane::Impl
{
    DockArea dockArea { DockArea::Left };
    QAction* bindAction { nullptr };
    QCtmNavigationBar* navigationBar { nullptr };
    QScrollArea* area { nullptr };

    QWidget* titleBar { nullptr };
    QPushButton* closeBtn { nullptr };
    QLabel* title { nullptr };
    bool popup { true };

    QWidget* initTitleBar(QWidget* parent)
    {
        titleBar = new QWidget(parent);
        titleBar->setObjectName("qcustomui_titleBar");
        QHBoxLayout* layout = new QHBoxLayout(titleBar);
        closeBtn            = new QPushButton(titleBar);
        layout->setContentsMargins(0, 0, 0, 0);
        closeBtn->setObjectName("qcustomui_closeBtn");
        title = new QLabel(titleBar);
        title->setObjectName("qcustomui_title");
        layout->addWidget(title, 1);
        layout->addWidget(closeBtn);
        return titleBar;
    }
};

/*!
    \class      QCtmNavigationSidePane
    \brief      边缘停靠窗口.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmNavigationSidePane.h

    \b          {截图:}
    \image      QCtmNavigationPaneDetail.png
*/

/*!
    \enum       QCtmNavigationSidePane::DockArea
                停靠位置.
    \value      Left
                窗口左侧.
    \value      Top
                窗口顶部.
    \value      Right
                窗口右侧.
    \value      Bottom
                窗口底部.
*/

/*!
    \fn         QCtmNavigationSidePane::paneClosed()
    \brief      停靠窗口关闭时发送该信号.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmNavigationSidePane::QCtmNavigationSidePane(QCtmNavigationBar* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setMinimumSize(10, 10);
    m_impl->navigationBar = parent;
    m_impl->area          = new QScrollArea(this);
    m_impl->area->setAutoFillBackground(false);
    m_impl->area->setFrameStyle(QFrame::NoFrame);
    m_impl->area->viewport()->setAutoFillBackground(false);

    m_impl->area->setWidgetResizable(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, margin, 0);
    layout->addWidget(m_impl->initTitleBar(this), 0);
    layout->addWidget(m_impl->area, 1);
    m_impl->titleBar->hide();
    if (parent)
        parent->window()->installEventFilter(this);

    connect(m_impl->closeBtn, &QPushButton::clicked, this, [=]() { this->close(); });
    this->setAttribute(Qt::WA_TranslucentBackground);
    setPopup(true);
}

/*!
    \brief      构造函数，构造时确定其停靠位置 \a area, \a parent.
*/
QCtmNavigationSidePane::QCtmNavigationSidePane(DockArea area, QCtmNavigationBar* parent) : QCtmNavigationSidePane(parent)
{
    setDockArea(area);
}

/*!
    \brief      析构函数.
*/
QCtmNavigationSidePane::~QCtmNavigationSidePane() {}

/*!
    \brief      设置窗口停靠位置 \a area.
    \sa         dockArea
*/
void QCtmNavigationSidePane::setDockArea(DockArea area)
{
    m_impl->dockArea = area;
    switch (area)
    {
    case DockArea::Left:
        layout()->setContentsMargins(0, 0, margin, 0);
        break;
    case DockArea::Right:
        layout()->setContentsMargins(margin, 0, 0, 0);
        break;
    case DockArea::Bottom:
        layout()->setContentsMargins(0, margin, 0, 0);
        break;
    case DockArea::Top:
        layout()->setContentsMargins(0, 0, 0, margin);
        break;
    }
}

/*!
    \brief      返回窗口停靠位置.
    \sa         setDockArea
*/
QCtmNavigationSidePane::DockArea QCtmNavigationSidePane::dockArea() const { return m_impl->dockArea; }

/*!
    \brief      设置要显示的窗口 \a widget.
    \note       \a widget 必须在设置前布局，设置窗口后再布局无效.
    \sa         widget()
*/
void QCtmNavigationSidePane::setWidget(QWidget* widget) { m_impl->area->setWidget(widget); }

/*!
    \brief      返回设置的窗口.
    \sa         setWidget
*/
QWidget* QCtmNavigationSidePane::widget() const { return m_impl->area->widget(); }

/*!
    \brief      返回容器窗口.
    \sa         widget()
*/
QScrollArea* QCtmNavigationSidePane::viewContainer() const { return m_impl->area; }

/*!
    \brief      设置标题栏是否可见 \a visible.
    \sa         titleBarIsVisible()
*/
void QCtmNavigationSidePane::setTitleBarVisible(bool visible) { m_impl->titleBar->setVisible(visible); }

/*!
    \brief      返回标题栏是否可见.
    \sa         setTitleBarVisible
*/
bool QCtmNavigationSidePane::titleBarIsVisible() const { return m_impl->titleBar->isVisible(); }

/*!
    \brief      设置标题栏文字 \a text.
    \sa         title()
*/
void QCtmNavigationSidePane::setTitle(const QString& text) { m_impl->title->setText(text); }

/*!
    \brief      返回标题栏文字.
    \sa         setTitle
*/
QString QCtmNavigationSidePane::title() const { return m_impl->title->text(); }

/*!
    \brief      设置窗口是否以Popup方式显示 \a popup.
    \sa         popup()
*/
void QCtmNavigationSidePane::setPopup(bool popup)
{
    m_impl->popup = popup;
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | (popup ? Qt::Popup : Qt::Tool));
}

/*!
    \brief      返回窗口是否以Popup方式显示.
    \sa         setPopup
*/
bool QCtmNavigationSidePane::popup() const { return m_impl->popup; }

/*!
    \reimp
*/
void QCtmNavigationSidePane::paintEvent([[maybe_unused]] QPaintEvent* event) { paintShadow(margin); }

/*!
    \reimp
*/
void QCtmNavigationSidePane::showEvent([[maybe_unused]] QShowEvent* event)
{
    if (!m_impl->navigationBar)
        return;
    auto pos = smartPosition(dockArea());
    move(pos);
    resize(sizeHint());
}

/*!
    \reimp
*/
void QCtmNavigationSidePane::closeEvent([[maybe_unused]] QCloseEvent* event) { emit paneClosed(); }

/*!
    \reimp
*/
QSize QCtmNavigationSidePane::sizeHint() const
{
    if (!m_impl->navigationBar)
        return QWidget::sizeHint();
    return smartSize(m_impl->dockArea);
}

/*!
    \brief      根据 \a area 自动计算窗口显示大小.
    \sa         smartPosition
*/
QSize QCtmNavigationSidePane::smartSize(DockArea area) const
{
    const auto& oldSize = QWidget::sizeHint();
    if (!m_impl->navigationBar)
        return oldSize;
    QSize alignSize;
    const auto& bindActionRect = m_impl->navigationBar->actionRect(m_impl->bindAction);
    auto topWidget             = m_impl->navigationBar->window();
    const auto& margins        = topWidget->layout()->contentsMargins();
    switch (area)
    {
    case DockArea::Left:
        alignSize.setWidth(oldSize.width() > bindActionRect.right() ? oldSize.width() : bindActionRect.right());
        if (alignSize.width() > m_impl->navigationBar->width() / 2)
            alignSize.setWidth(m_impl->navigationBar->width() / 2);
        alignSize.setHeight(topWidget->height() - m_impl->navigationBar->y() - m_impl->navigationBar->height() - margins.bottom());
        break;
    case DockArea::Right:
        alignSize.setWidth(oldSize.width() > m_impl->navigationBar->width() - bindActionRect.left()
                               ? oldSize.width()
                               : m_impl->navigationBar->width() - bindActionRect.left());
        if (alignSize.width() > m_impl->navigationBar->width() / 2)
            alignSize.setWidth(m_impl->navigationBar->width() / 2);
        alignSize.setHeight(topWidget->height() - m_impl->navigationBar->y() - m_impl->navigationBar->height() - margins.bottom());
        break;
    case DockArea::Top:
    case DockArea::Bottom:
        {
            alignSize.setWidth(m_impl->navigationBar->width());
            auto h = oldSize.height() > minimumHeight() ? oldSize.height() : minimumHeight();
            alignSize.setHeight(h > topWidget->height() / 2 ? topWidget->height() / 2 : h);
            break;
        }
    }
    return alignSize.boundedTo(maximumSize());
}

/*!
    \brief      绑定 QCtmNavigationBar \a bar.
    \sa         navigationBar(), QCtmNavigationBar
*/
void QCtmNavigationSidePane::setNavigationBar(QCtmNavigationBar* bar)
{
    if (m_impl->navigationBar)
    {
        m_impl->navigationBar->window()->removeEventFilter(this);
    }
    setParent(bar);
    setPopup(m_impl->popup);
    m_impl->navigationBar = bar;
    bar->window()->installEventFilter(this);
}

/*!
    \brief      返回绑定的 QCtmNavigationBar.
    \sa         setNavigationBar, QCtmNavigationBar
*/
QCtmNavigationBar* QCtmNavigationSidePane::navigationBar() const { return m_impl->navigationBar; }

/*!
    \reimp
*/
bool QCtmNavigationSidePane::eventFilter(QObject* o, QEvent* e)
{
    if (m_impl->navigationBar && o == m_impl->navigationBar->window())
    {
        switch (e->type())
        {
        case QEvent::Move:
        case QEvent::Resize:
            {
                auto pos = smartPosition(dockArea());
                move(pos);
                resize(sizeHint());
                break;
            }
#if WIN32 // WIN10 最大化计算bugfix
        case QEvent::WindowStateChange:
            {
                QTimer::singleShot(1,
                                   [=]
                                   {
                                       auto pos = smartPosition(dockArea());
                                       move(pos);
                                       resize(sizeHint());
                                   });
            }
            break;
#endif
        default:
            break;
        }
    }
    return false;
}

/*!
    \reimp
*/
void QCtmNavigationSidePane::mousePressEvent(QMouseEvent* event)
{
    event->ignore();
    if ((windowType() == Qt::Popup))
    {
        event->accept();
        QWidget* w;
        while ((w = QApplication::activePopupWidget()) && w != this)
        {
            w->close();
            if (QApplication::activePopupWidget() == w) // widget does not want to disappear
                w->hide();                              // hide at least
        }
        if (!rect().contains(event->pos()))
        {
            close();
        }
    }
}

/*!
    \brief      绑定到 QCtmNavigationBar 上的 \a action.
    \sa         QCtmNavigationBar
*/
void QCtmNavigationSidePane::bindAction(QAction* action) { m_impl->bindAction = action; }

/*!
    \brief      绘制阴影 \a shadowWidth.
    \sa         paintEvent
*/
void QCtmNavigationSidePane::paintShadow(int shadowWidth)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient line;
    line.setColorAt(0, QColor(0, 0, 0, 50));
    line.setColorAt(1, QColor(0, 0, 0, 0));

    QStyleOption opt;
    opt.initFrom(this);

    switch (m_impl->dockArea)
    {
    case DockArea::Left:
        opt.rect = { 0, 0, this->width() - shadowWidth, this->height() };
        line.setStart(this->width() - shadowWidth, 0);
        line.setFinalStop(this->width(), 0);
        painter.fillRect(this->width() - shadowWidth, 0, shadowWidth, height(), QBrush(line));
        break;
    case DockArea::Right:
        opt.rect = { shadowWidth, 0, this->width() - shadowWidth, this->height() };
        line.setStart(shadowWidth, 0);
        line.setFinalStop(0, 0);
        painter.fillRect(0, 0, shadowWidth, height(), QBrush(line));
        break;
    case DockArea::Top:
        opt.rect = { 0, 0, this->width(), this->height() - shadowWidth };
        line.setStart(0, this->height() - shadowWidth);
        line.setFinalStop(0, this->height());
        painter.fillRect(0, this->height() - shadowWidth, width(), shadowWidth, QBrush(line));
        break;
    case DockArea::Bottom:
        opt.rect = { 0, shadowWidth, this->width(), this->height() - shadowWidth };
        line.setStart(0, shadowWidth);
        line.setFinalStop(0, 0);
        painter.fillRect(0, 0, width(), shadowWidth, QBrush(line));
        break;
    }

    painter.fillRect(opt.rect, opt.palette.window());
}

/*!
    \brief      根据 \a area 计算显示位置.
    \sa         smartSize
*/
QPoint QCtmNavigationSidePane::smartPosition(DockArea area) const
{
    if (!m_impl->navigationBar)
        return QPoint(0, 0);
    auto topWidget      = m_impl->navigationBar->window();
    auto pos            = topWidget->mapToGlobal(m_impl->navigationBar->pos());
    const auto& margins = topWidget->layout()->contentsMargins();
    switch (area)
    {
    case DockArea::Left:
        return { pos.x(), pos.y() + m_impl->navigationBar->height() };
    case DockArea::Right:
        return { pos.x() + topWidget->width() - sizeHint().width() - margins.left() - margins.right(),
                 pos.y() + m_impl->navigationBar->height() };
    case DockArea::Top:
        return { pos.x(), pos.y() + m_impl->navigationBar->height() };
    case DockArea::Bottom:
        return { pos.x(), topWidget->y() + topWidget->height() - sizeHint().height() - margins.bottom() };
    }
    return {};
}
