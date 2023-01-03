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

#include "QCtmTitleBar.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"
#include "ui_QCtmTitleBar.h"

#include <QMenuBar>
#include <QPainter>
#include <QPointer>
#include <QResizeEvent>
#include <QStyleOption>

Q_CONSTEXPR int leftMargin   = 5;
Q_CONSTEXPR int titleSpacing = 5;

struct QCtmTitleBar::Impl
{
    QPixmap windowIcon;
    QPointer<QMenuBar> menuBar;
    bool showIcon { true };
    QList<QCtmWidgetItemPtr> items;
    QSize iconSize { 16, 16 };
};

/*!
    \class      QCtmTitleBar
    \brief      QCtmWindow 和 QCtmDialog 的自定义标题栏.
    \note       最大化，最小化，关闭按钮等显示控制由 QWidget::windowFlags 决定，图标显示由 QWidget::windowIcon
                控制，标题栏文字由centralWidget的QWidget::windowTitle决定.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmTitleBar.h

    \b          {截图:}
    \image      QCtmTitleBarDetail.png
*/

/*!
    \property   QCtmTitleBar::iconIsVisible
    \brief      是否显示图标.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmTitleBar::QCtmTitleBar(QWidget* parent) : QWidget(parent), ui(new Ui::QCtmTitleBar), m_impl(std::make_unique<Impl>())
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->closeBtn, &QPushButton::clicked, this, &QCtmTitleBar::onCloseBtn);
    connect(ui->minimumSizeBtn, &QPushButton::clicked, this, &QCtmTitleBar::onMinimumSizeBtn);
    connect(ui->maximumSizeBtn, &QPushButton::clicked, this, &QCtmTitleBar::onMaximumSizeBtn);
    ui->maximumSizeBtn->setProperty("qcustomui_maximumSizeButton", true);

    parent->installEventFilter(this);
    setAttribute(Qt::WA_StyledBackground);
}

/*!
    \brief      析构函数.
*/
QCtmTitleBar::~QCtmTitleBar() { delete ui; }

/*!
    \brief      设置菜单栏 \a menu, 当 \a menu 为nullptr时删除菜单栏.
    \sa         menuBar
*/
void QCtmTitleBar::setMenuBar(QMenuBar* menu)
{
    if (menu == m_impl->menuBar)
        return;
    if (m_impl->menuBar)
    {
        m_impl->menuBar->hide();
        m_impl->menuBar->setParent(nullptr);
        m_impl->menuBar->deleteLater();
    }
    if (menu)
    {
        menu->setAutoFillBackground(false);
        menu->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
        ui->horizontalLayout->insertWidget(0, menu);
    }
    m_impl->menuBar = menu;
    update();
}

/*!
    \brief      返回菜单栏, 当菜单栏对象不存在时，自动创建并返回一个新的菜单栏.
    \sa         setMenuBar
*/
QMenuBar* QCtmTitleBar::menuBar() const
{
    auto menuBar = m_impl->menuBar;
    if (!menuBar)
    {
        auto self = const_cast<QCtmTitleBar*>(this);
        menuBar   = new QMenuBar(self);
        self->setMenuBar(menuBar);
    }
    return menuBar;
}

/*!
    \brief      设置是否显示图标 \a show.
    \sa         iconIsVisible()
*/
void QCtmTitleBar::setIconVisible(bool show)
{
    m_impl->showIcon = show;
    if (show)
        ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->windowIcon.width(), 0, 0, 0);
    else
        ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
}

/*!
    \brief      返回是否显示图标.
    \sa         setIconVisible
*/
bool QCtmTitleBar::iconIsVisible() const { return m_impl->showIcon; }

/*!
    \brief      设置Action图标大小 \a size.
    \sa         iconSize
*/
void QCtmTitleBar::setIconSize(const QSize& size) { m_impl->iconSize = size; }

/*!
    \brief      返回Action图标大小.
    \sa         setIconSize
*/
const QSize& QCtmTitleBar::iconSize() const { return m_impl->iconSize; }

/*!
    \brief      响应关闭按钮.
    \sa         onMaximumSizeBtn(), onMinimumSizeBtn()
*/
void QCtmTitleBar::onCloseBtn() { this->window()->close(); }

/*!
    \brief      响应最大化按钮.
    \sa         onCloseBtn(), onMinimumSizeBtn()
*/
void QCtmTitleBar::onMaximumSizeBtn()
{
    if (this->window()->isMaximized() || this->window()->isFullScreen())
        this->window()->showNormal();
    else
        this->window()->showMaximized();
}

/*!
    \brief      响应最小化按钮.
    \sa         onCloseBtn(), onMaximumSizeBtn()
*/
void QCtmTitleBar::onMinimumSizeBtn() { this->window()->showMinimized(); }

/*!
    \reimp
*/
void QCtmTitleBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    const auto& iconRect = doIconRect();
    if (!m_impl->windowIcon.isNull() && m_impl->showIcon)
    {
        p.drawPixmap(iconRect, m_impl->windowIcon);
    }

    if (parentWidget())
    {
        auto text = parentWidget()->windowTitle();
        QRect rect(QPoint { 0, 0 }, opt.fontMetrics.size(Qt::TextSingleLine, text));
        if (m_impl->menuBar)
        {
            if (m_impl->menuBar->geometry().right() >= (this->width() - rect.width()) / 2)
            {
                rect.moveTo((this->width() - m_impl->menuBar->geometry().right() - rect.width()) / 2, 0);
            }
            else
                rect.moveTo((width() - rect.width()) / 2, 0);
        }
        else
            rect.moveTo(leftMargin + iconRect.width() + titleSpacing, 0);
        rect.setHeight(this->height());
        QTextOption to;
        to.setWrapMode(QTextOption::NoWrap);
        to.setAlignment(Qt::AlignCenter);
        p.setFont(this->font());
        p.setPen(this->palette().windowText().color());
        p.drawText(rect, text, to);
    }
}

/*!
    \reimp
*/
void QCtmTitleBar::showEvent([[maybe_unused]] QShowEvent* event)
{
    auto w = qobject_cast<QWidget*>(this->parent());
    if (w)
    {
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowCloseButtonHint))
            ui->closeBtn->hide();
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowMinimizeButtonHint))
            ui->minimumSizeBtn->hide();
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowMaximizeButtonHint))
            ui->maximumSizeBtn->hide();
        if (w->windowState().testFlag(Qt::WindowState::WindowMaximized))
            ui->maximumSizeBtn->setMaximumSized(true);
        else
            ui->maximumSizeBtn->setMaximumSized(false);
    }
}

/*!
    \reimp
*/
bool QCtmTitleBar::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == parent())
    {
        auto w = qobject_cast<QWidget*>(this->parent());
        if (!w)
            return false;
        if (event->type() == QEvent::WindowStateChange)
        {
            auto w = qobject_cast<QWidget*>(this->parent());
            if (w)
            {
                if (w->windowState().testFlag(Qt::WindowState::WindowMaximized))
                    ui->maximumSizeBtn->setMaximumSized(true);
                else
                    ui->maximumSizeBtn->setMaximumSized(false);
            }
        }
        else if (event->type() == QEvent::WindowIconChange)
        {
            auto sizes = w->windowIcon().availableSizes();
            if (!sizes.isEmpty())
            {
                m_impl->windowIcon = w->windowIcon().pixmap(sizes.front());
#if QT_VERSION <= QT_VERSION_CHECK(5, 8, 0)
                int size = 16;
#else
                int size = this->style()->pixelMetric(QStyle::PM_TitleBarButtonIconSize);
#endif
                if (m_impl->windowIcon.height() > size)
                {
                    m_impl->windowIcon = m_impl->windowIcon.scaledToHeight(size, Qt::SmoothTransformation);
                }
                if (m_impl->showIcon)
                    ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->windowIcon.width(), 0, 0, 0);
            }
        }
        else if (event->type() == QEvent::WindowTitleChange)
        {
            update();
        }
        else if (event->type() == QEvent::Resize)
        {
            if (w->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
            {
                if (w->maximumSize() == w->minimumSize())
                    ui->maximumSizeBtn->hide();
                else
                    ui->maximumSizeBtn->show();
            }
        }
    }
    return false;
}

/*!
    \reimp
*/
void QCtmTitleBar::actionEvent(QActionEvent* event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCtmTitleBar::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
        Util::addItem(item, m_impl->items, event->before(), ui->actionLayout);
    }
    else
    {
        Util::removeItem(event->action(), m_impl->items, ui->actionLayout);
    }
}

/*!
    \brief      计算图标包围盒.
*/
QRect QCtmTitleBar::doIconRect() const
{
    return { leftMargin, (this->height() - m_impl->windowIcon.height()) / 2, m_impl->windowIcon.width(), m_impl->windowIcon.height() };
}
