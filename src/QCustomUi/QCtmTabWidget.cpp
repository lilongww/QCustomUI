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

#include "QCtmTabWidget.h"
#include "QCtmTabPage.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QToolButton>

struct QCtmTabWidget::Impl
{
    QWidget* cornerWidget { nullptr };
    QHBoxLayout* cornerLayout { nullptr };
    QToolButton* closeBtn { nullptr };
    QSize iconSize { 16, 16 };

    void setPageCornerWidget(QWidget* widget)
    {
        if (cornerLayout->itemAt(0))
        {
            if (cornerLayout->itemAt(0)->widget() != closeBtn)
            {
                auto item = cornerLayout->takeAt(0);
                if (item->widget())
                    item->widget()->hide();
            }
        }
        cornerLayout->insertWidget(0, widget);
    }
};

/*!
    \class      QCtmTabWidget
    \brief      与QTableWidget相似，扩展了角落窗口的功能.
    \inherits   QTabWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmTabWidget.h
*/

/*!
    \fn         void QCtmTabWidget::iconSizeChanged(const QSize& size);
    \brief      当Action的图标大小发生变化时发送该信号 \a size.
    \sa         setIconSize
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmTabWidget::QCtmTabWidget(QWidget* parent) : QTabWidget(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->cornerWidget = new QWidget(this);
    m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
    m_impl->cornerLayout->setContentsMargins(0, 0, 0, 0);

    m_impl->closeBtn = new QToolButton(this);
    m_impl->closeBtn->setObjectName("closeButton");
    m_impl->cornerLayout->addWidget(m_impl->closeBtn);

    setCornerWidget(m_impl->cornerWidget);
    connect(this, &QTabWidget::currentChanged, this, &QCtmTabWidget::onCurrentChanged);
    connect(m_impl->closeBtn, &QToolButton::clicked, this, &QWidget::hide);
    setAttribute(Qt::WA_StyledBackground);
}

/*!
    \brief      析构函数.
*/
QCtmTabWidget::~QCtmTabWidget() {}

/*!
    \overload   addTab
                添加标题为 \a label 的子页面 \a widget.
    \sa         QCtmTabWidget::addTab
*/
QCtmTabPage* QCtmTabWidget::addTab(QWidget* widget, const QString& label) { return addTab(widget, QIcon(), label); }

/*!
    \overload   addTab
    \brief      添加图标为 \a icon 标题为 \a label 的子页面 \a widget.
    \sa         QCtmTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::addTab(QWidget* widget, const QIcon& icon, const QString& label)
{
    return insertTab(count(), widget, icon, label);
}

/*!
    \overload   insertTab
                在 \a index 的位置插入标题为 \a label 的子页面 \a widget.
    \sa         QCtmTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget* widget, const QString& label) { return insertTab(index, widget, QIcon(), label); }

/*!
    \overload   insertTab
                在 \a index 的位置插入图标为 \a icon 标题为 \a label 的子页面 \a widget.
    \sa         QTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget* widget, const QIcon& icon, const QString& label)
{
    auto page = new QCtmTabPage(this);
    page->setIconSize(m_impl->iconSize);
    connect(this, &QCtmTabWidget::iconSizeChanged, page, &QCtmTabPage::setIconSize);
    page->setCentralWidget(widget);
    QTabWidget::insertTab(index, page, icon, label);
    return page;
}

/*!
    \brief      设置Action的图标大小 \a size.
    \sa         iconSize
*/
void QCtmTabWidget::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(size);
}

/*!
    \brief      返回Action的图标大小.
    \sa         setIconSize
*/
const QSize& QCtmTabWidget::iconSize() const { return m_impl->iconSize; }

/*!
    \brief      切换页面 \a index 时修改角落窗口的显示内容.
*/
void QCtmTabWidget::onCurrentChanged(int index)
{
    auto w    = this->widget(index);
    auto page = qobject_cast<QCtmTabPage*>(w);
    if (page)
    {
        m_impl->setPageCornerWidget(page->cornerWidget());
        page->cornerWidget()->show();
    }
    else
        this->setCornerWidget(nullptr);
}
