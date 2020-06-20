/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2020 LiLong                                              **
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
#include <QPainter>
#include <QStyleOption>
#include <QEvent>
#include <QHBoxLayout>
#include <QToolButton>

struct QCtmTabWidget::Impl
{
    QWidget* cornerWidget{ nullptr };
    QHBoxLayout* cornerLayout{ nullptr };
    QToolButton* closeBtn{ nullptr };

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
    \brief      QCtmTabWidget provide a tab widget, that can add action to the top right area.
    \inherits   QTabWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
*/

/*!
    \brief      Constructs a tab widget with the given \a parent.
*/
QCtmTabWidget::QCtmTabWidget(QWidget* parent) :
    QTabWidget(parent),
    m_impl(std::make_unique<Impl>())
{
    m_impl->cornerWidget = new QWidget(this);
    m_impl->cornerLayout = new QHBoxLayout(m_impl->cornerWidget);
    m_impl->cornerLayout->setMargin(0);

    m_impl->closeBtn = new QToolButton(this);
    m_impl->closeBtn->setObjectName("closeButton");
    m_impl->cornerLayout->addWidget(m_impl->closeBtn);

    setCornerWidget(m_impl->cornerWidget);
    connect(this, &QTabWidget::currentChanged, this, &QCtmTabWidget::onCurrentChanged);
    connect(m_impl->closeBtn, &QToolButton::clicked, this, &QWidget::hide);
}

/*!
    \brief      Destroys the tab widget.
*/
QCtmTabWidget::~QCtmTabWidget()
{

}

/*!
    \overload   addTab
                Add a tab to the tab widget with the given \a widget and \a label.
                And returns the page.
    \sa         QCtmTabWidget::addTab
*/
QCtmTabPage* QCtmTabWidget::addTab(QWidget* widget, const QString& label)
{
    return addTab(widget, QIcon(), label);
}

/*!
    \brief      Add a tab to the tab widget with the given \a widget, \a icon and \a label.
                And returns the page.
    \sa         QCtmTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::addTab(QWidget* widget, const QIcon& icon, const QString& label)
{
    return insertTab(count(), widget, icon, label);
}

/*!
    \overload   insertTab
                Insert a tab to the tab widget with the given \a index, \a widget and \a label.
                And returns the page.
    \sa         QCtmTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget* widget, const QString& label)
{
    return insertTab(index, widget, QIcon(), label);
}

/*!
    \overload   insertTab
                Insert a tab to the tab widget with the given \a index, \a widget, \a icon and \a label.
                And returns the page.
    \sa         QTabWidget::insertTab
*/
QCtmTabPage* QCtmTabWidget::insertTab(int index, QWidget* widget, [[maybe_unused]] const QIcon& icon, const QString& label)
{
    auto page = new QCtmTabPage(this);
    page->setCentralWidget(widget);
    QTabWidget::insertTab(index, page, label);
    return page;
}

/*!
    \reimp
*/
void QCtmTabWidget::paintEvent([[maybe_unused]] QPaintEvent* e)
{
    QPainter p(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/*!
    \brief      Change the corner widget when the current \a index changed.
*/
void QCtmTabWidget::onCurrentChanged(int index)
{
    auto w = this->widget(index);
    auto page = qobject_cast<QCtmTabPage*>(w);
    if (page)
    {
        m_impl->setPageCornerWidget(page->cornerWidget());
        page->cornerWidget()->show();
    }
    else
        this->setCornerWidget(nullptr);
}

