/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2026 LiLong                                              **
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
#include "Private/QCtmPaletteFactor.h"
#include "Private/QCtmWidgetItem_p.h"
#include "Private/Util_p.h"
#include "ui_QCtmTitleBar.h"

#include <QMenuBar>
#include <QPainter>
#include <QPointer>
#include <QResizeEvent>
#include <QStyle>
#include <QStyleOption>

Q_CONSTEXPR int leftMargin   = 5;
Q_CONSTEXPR int titleSpacing = 5;

struct QCtmTitleBar::Impl
{
    QPointer<QMenuBar> menuBar;
    bool showIcon { true };
    QList<QCtmWidgetItemPtr> items;
    QSize iconSize { 24, 24 };
    Qt::Alignment titleAlignment { Qt::AlignLeft | Qt::AlignVCenter };
};

/*!
    \class      QCtmTitleBar
    \brief      QCtmWindow 和 QCtmDialog 的自定义标题栏.
    \note       最大化，最小化，关闭按钮等显示控制由 QWidget::windowFlags 决定，图标显示由 QWidget::windowIcon
                控制，标题栏文字由centralWidget的QWidget::windowTitle决定.
    \inherits   QCtmAbstractTitleBar
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
    \property   QCtmTitleBar::iconSize
    \brief      Action图标大小.
*/

/*!
    \property   QCtmTitleBar::windowTitleAlignment
    \brief      窗口标题对齐方式.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmTitleBar::QCtmTitleBar(QWidget* parent) : QCtmAbstractTitleBar(parent), ui(new Ui::QCtmTitleBar), m_impl(std::make_unique<Impl>())
{
    ui->setupUi(this);
    QCtmPaletteFactor::init(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->closeBtn, &QAbstractButton::clicked, this, &QCtmTitleBar::onCloseButtonClicked);
    connect(ui->minimumSizeBtn, &QAbstractButton::clicked, this, &QCtmTitleBar::onMinimizeButtonClicked);
    connect(ui->maximumSizeBtn, &QAbstractButton::clicked, this, &QCtmTitleBar::onMaximizeButtonClicked);
    ui->maximumSizeBtn->setProperty("qcustomui_maximumSizeButton", true);

    parent->installEventFilter(this);
    setAttribute(Qt::WA_StyledBackground);
    QStyleOption opt;
    opt.initFrom(this);
    auto h = this->style()->pixelMetric(QStyle::PM_TitleBarHeight, &opt, this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) //  Qt6.7更改了pixelMetric PM_TitleBarHeight的算法，返回的数字是计算分辨率缩放之后的。
    h = std::max<int>(std::ceil(this->devicePixelRatio() * h),
                      32); // https://learn.microsoft.com/zh-cn/windows/apps/design/basics/titlebar-design
#endif
    setFixedHeight(h);
}

/*!
    \brief      析构函数.
*/
QCtmTitleBar::~QCtmTitleBar()
{
    delete ui;
}

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

    if (m_impl->showIcon)
        ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->iconSize.width(), 0, 0, 0);
    else
        ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
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
        ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->iconSize.width(), 0, 0, 0);
    else
        ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
}

/*!
    \brief      返回是否显示图标.
    \sa         setIconVisible
*/
bool QCtmTitleBar::iconIsVisible() const
{
    return m_impl->showIcon;
}

/*!
    \brief      设置Action图标大小 \a size.
    \sa         iconSize
*/
void QCtmTitleBar::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
}

/*!
    \brief      返回Action图标大小.
    \sa         setIconSize
*/
const QSize& QCtmTitleBar::iconSize() const
{
    return m_impl->iconSize;
}

/*!
    \brief      设置窗口标题对齐方式 \a alignment.
    \sa         windowTitleAlignment
*/
void QCtmTitleBar::setWindowTitleAlignment(Qt::Alignment alignment)
{
    m_impl->titleAlignment = alignment;
    update();
}

/*!
    \brief      返回窗口标题对齐方式.
    \sa         setWindowTitleAlignment
*/
Qt::Alignment QCtmTitleBar::windowTitleAlignment() const
{
    return m_impl->titleAlignment;
}

/*!
    \reimp
*/
void QCtmTitleBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    const auto& iconRect = doIconRect();
    if (auto w = this->window(); w && m_impl->showIcon)
    {
        w->windowIcon().paint(&p, iconRect);
    }

    if (parentWidget())
    {
        auto text = parentWidget()->windowTitle();
        int left  = iconRect.right() + titleSpacing;
        if (m_impl->menuBar)
        {
            left = m_impl->menuBar->geometry().right() + titleSpacing;
        }
        int right = ui->actionLayout->geometry().left() - titleSpacing;
        QRect rect(left, 0, right - left, this->height());

        if (m_impl->titleAlignment & Qt::AlignHCenter)
        {
            auto textRect = this->fontMetrics().boundingRect(text);
            rect.setLeft((this->width() - textRect.width()) / 2);
            rect.setRight(rect.left() + textRect.width());
            if (rect.left() < left)
                rect.setLeft(left);
        }
        QTextOption to;
        to.setWrapMode(QTextOption::NoWrap);
        to.setAlignment(m_impl->titleAlignment);
        p.setFont(this->font());
        p.setPen(this->palette().windowText().color());
        p.drawText(rect, text, to);
    }
}

/*!
    \reimp
*/
void QCtmTitleBar::actionEvent(QActionEvent* event)
{
    switch (event->type())
    {
    case QEvent::ActionAdded:
        {
            auto item = std::make_shared<QCtmWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
            connect(this, &QCtmTitleBar::iconSizeChanged, item.get(), &QCtmWidgetItem::iconSizeChanged);
            Util::addItem(item, m_impl->items, event->before(), ui->actionLayout);
        }
        break;
    case QEvent::ActionRemoved:
        Util::removeItem(event->action(), m_impl->items, ui->actionLayout);
        break;
    }
}

/*!
    \brief      计算图标包围盒.
*/
QRect QCtmTitleBar::doIconRect() const
{
    QStyleOption opt;
    opt.initFrom(this);
    auto rect = this->style()->subElementRect(QStyle::SE_FrameContents, &opt, this);
    if (!m_impl->showIcon)
        return QRect();
    else if (!rect.isValid())
        rect = this->rect();
    auto icon = this->windowIcon();
    auto size = icon.actualSize(rect.size());
    return { leftMargin, (this->height() - size.height()) / 2, size.width(), size.height() };
}

/*!
    \reimp
*/
void QCtmTitleBar::onWindowMaximized(bool isMaximumSized)
{
    ui->maximumSizeBtn->setMaximumSized(isMaximumSized);
}

/*!
    \reimp
*/
void QCtmTitleBar::onWindowMaximizeButtonHint(bool showMaximizeButton)
{
    ui->maximumSizeBtn->setVisible(showMaximizeButton);
}

/*!
    \reimp
*/
void QCtmTitleBar::onWindowCloseButtonHint(bool showCloseButton)
{
    ui->closeBtn->setVisible(showCloseButton);
}

/*!
    \reimp
*/
void QCtmTitleBar::onWindowMinimizeButtonHint(bool showMinimizeButton)
{
    ui->minimumSizeBtn->setVisible(showMinimizeButton);
}

/*!
    \reimp
*/
void QCtmTitleBar::onWindowTitleChanged(const QString& title)
{
    Q_UNUSED(title);
    update();
}

/*!
    \reimp
*/
bool QCtmTitleBar::showIconSystemMenu(const QPoint& pos) const
{
    return this->iconIsVisible() && this->doIconRect().contains(pos);
}