/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2022 LiLong                                              **
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
#include "QCtmMultiPageButtonBox.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

struct QCtmMultiPageButtonBox::Impl
{
    QCtmAbstractMultiPageItemModel* model { nullptr };
    int pageButtonCount { 5 };
    bool pageButtonVisible { true };
    QPushButton* prev { nullptr };
    QPushButton* next { nullptr };
    QHBoxLayout* pageButtonLayout { nullptr };
    QLabel* total { nullptr };
    QSpinBox* currentPage { nullptr };
};

/*!
    \class      QCtmMultiPageButtonBox
    \brief      分页按钮组合.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmMultiPageButtonBox.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmMultiPageButtonBox::QCtmMultiPageButtonBox(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>()) { init(); }

/*!
    \brief      析构函数.
*/
QCtmMultiPageButtonBox::~QCtmMultiPageButtonBox() {}

/*!
    \brief      设置数据来源 \a model.
    \sa         model
*/
void QCtmMultiPageButtonBox::setModel(QCtmAbstractMultiPageItemModel* model)
{
    if (m_impl->model == model)
        return;
}

/*!
    \brief      返回数据源.
    \sa         setModel
*/
QCtmAbstractMultiPageItemModel* QCtmMultiPageButtonBox::model() const { return m_impl->model; }

/*!
    \brief      设置页面按钮数量 \a buttonCount，如果实际页面数量少于/设置的数量，则显示的页面按钮数量为实际数量，
                如果实际页面的数量多于页面按钮数量，则显示页面按钮的数量为设置的数量.
    \sa
*/
void QCtmMultiPageButtonBox::setPageButtonCount(int buttonCount)
{
    m_impl->pageButtonCount = buttonCount;
    if (!m_impl->pageButtonVisible)
        return;
}

/*!
    \brief      返回页面按钮的设置数量.
    \sa
*/
int QCtmMultiPageButtonBox::pageButtonCount() const { return m_impl->pageButtonCount; }

/*!
    \brief      设置页面按钮是否可见 \a visible.
    \sa         pageButtonVisible
*/
void QCtmMultiPageButtonBox::setPageButtonVisible(bool visible)
{
    if (visible == m_impl->pageButtonVisible)
        return;
    m_impl->pageButtonVisible = visible;
}

/*!
    \brief      返回页面按钮是否可见.
    \sa         setPageButtonVisible
*/
bool QCtmMultiPageButtonBox::pageButtonVisible() const { return m_impl->pageButtonVisible; }

void QCtmMultiPageButtonBox::init()
{
    auto layout = new QHBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(m_impl->prev = new QPushButton("<", this));
    layout->addLayout(m_impl->pageButtonLayout = new QHBoxLayout);
    layout->addWidget(m_impl->next = new QPushButton(">", this));
    layout->addWidget(m_impl->currentPage = new QSpinBox(this));
    m_impl->currentPage->setRange(0, 0);
    layout->addWidget(new QLabel("/"));
    layout->addWidget(m_impl->total = new QLabel("0"));
}

void QCtmMultiPageButtonBox::generatePageButtons() {}
