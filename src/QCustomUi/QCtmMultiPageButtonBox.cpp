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
#include "QCtmMultiPageButtonBox.h"
#include "QCtmAbstractMultiPageItemModel.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>

struct QCtmMultiPageButtonBox::Impl
{
    QCtmAbstractMultiPageItemModel* model { nullptr };
    int pageButtonCount { 5 };
    bool pageButtonVisible { true };
    int pageButtonBeginIndex { 0 };
    QPushButton* prev { nullptr };
    QPushButton* next { nullptr };
    QHBoxLayout* pageButtonLayout { nullptr };
    QLabel* total { nullptr };
    QSpinBox* currentPage { nullptr };
    QButtonGroup* pageButtonGroup { nullptr };
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
    if (m_impl->model)
    {
        disconnect(m_impl->model, &QCtmAbstractMultiPageItemModel::pageCountChanged, this, &QCtmMultiPageButtonBox::onPageCountChanged);
        disconnect(m_impl->model, &QCtmAbstractMultiPageItemModel::currentPageChanged, this, &QCtmMultiPageButtonBox::onCurrentPageChanged);
        disconnect(model, &QCtmAbstractMultiPageItemModel::modelReset, this, &QCtmMultiPageButtonBox::onModelReset);
    }
    m_impl->model = model;
    if (model)
    {
        connect(model, &QCtmAbstractMultiPageItemModel::pageCountChanged, this, &QCtmMultiPageButtonBox::onPageCountChanged);
        connect(model, &QCtmAbstractMultiPageItemModel::currentPageChanged, this, &QCtmMultiPageButtonBox::onCurrentPageChanged);
        connect(model, &QCtmAbstractMultiPageItemModel::modelReset, this, &QCtmMultiPageButtonBox::onModelReset);
    }
    onPageCountChanged();
    onCurrentPageChanged();
}

/*!
    \brief      返回数据源.
    \sa         setModel
*/
QCtmAbstractMultiPageItemModel* QCtmMultiPageButtonBox::model() const { return m_impl->model; }

/*!
    \brief      设置页面按钮数量 \a buttonCount, 如果实际页面数量少于/设置的数量，则显示的页面按钮数量为实际数量，
                如果实际页面的数量多于页面按钮数量，则显示页面按钮的数量为设置的数量，将页面按钮数量设为0时，不显示页面按钮.
    \sa         pageButtonCount
*/
void QCtmMultiPageButtonBox::setPageButtonCount(int buttonCount)
{
    m_impl->pageButtonCount = buttonCount;
    if (!m_impl->pageButtonVisible)
        return;
    onPageCountChanged();
    onCurrentPageChanged();
}

/*!
    \brief      返回页面按钮的设置数量.
    \sa         setPageButtonCount
*/
int QCtmMultiPageButtonBox::pageButtonCount() const { return m_impl->pageButtonCount; }

/*!
    \brief      返回上一页按钮的地址.
    \sa         nextButton
*/
QPushButton* QCtmMultiPageButtonBox::prevButton() const { return m_impl->prev; }

/*!
    \brief      返回下一页按钮的地址.
    \sa         prevButton
*/
QPushButton* QCtmMultiPageButtonBox::nextButton() const { return m_impl->next; }

void QCtmMultiPageButtonBox::init()
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::ButtonBox));
    auto layout = new QHBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(m_impl->prev = new QPushButton("<", this));
    layout->addLayout(m_impl->pageButtonLayout = new QHBoxLayout);
    layout->addWidget(m_impl->next = new QPushButton(">", this));
    layout->addWidget(m_impl->currentPage = new QSpinBox(this));
    m_impl->currentPage->setRange(0, 0);
    layout->addWidget(new QLabel("/"));
    layout->addWidget(m_impl->total = new QLabel("0"));
    m_impl->pageButtonGroup = new QButtonGroup(this);
    m_impl->pageButtonGroup->setExclusive(true);
    connect(m_impl->currentPage,
            &QSpinBox::editingFinished,
            this,
            [=]
            {
                if (m_impl->model)
                {
                    m_impl->model->setCurrentPage(m_impl->currentPage->value() - 1);
                }
            });
    connect(m_impl->pageButtonGroup,
            &QButtonGroup::idToggled,
            this,
            [=](auto id, bool checked)
            {
                if (m_impl->model && checked)
                {
                    m_impl->model->setCurrentPage(id + m_impl->pageButtonBeginIndex);
                }
            });
    connect(m_impl->prev,
            &QPushButton::clicked,
            this,
            [=]
            {
                if (m_impl->model)
                    m_impl->model->prev();
            });
    connect(m_impl->next,
            &QPushButton::clicked,
            this,
            [=]
            {
                if (m_impl->model)
                    m_impl->model->next();
            });
}

void QCtmMultiPageButtonBox::onPageCountChanged()
{
    if (!m_impl->model)
    {
        m_impl->currentPage->setRange(0, 0);
        m_impl->total->setText("0");
        auto btns = m_impl->pageButtonGroup->buttons();
        for (auto btn : btns)
        {
            m_impl->pageButtonLayout->removeWidget(btn);
            m_impl->pageButtonGroup->removeButton(btn);
            btn->hide();
            btn->deleteLater();
        }
        return;
    }
    auto beforeButtonCount = m_impl->pageButtonGroup->buttons().size();
    auto afterButtonCount  = std::min(m_impl->model->pageCount(), m_impl->pageButtonCount);
    if (beforeButtonCount < afterButtonCount)
    {
        for (int i = beforeButtonCount; i < afterButtonCount; ++i)
        {
            auto btn = new QToolButton(this);
            btn->setCheckable(true);
            m_impl->pageButtonGroup->addButton(btn, m_impl->pageButtonGroup->buttons().size());
            m_impl->pageButtonLayout->addWidget(btn);
        }
    }
    else
    {
        for (int i = afterButtonCount; i < beforeButtonCount; ++i)
        {
            auto btn = m_impl->pageButtonGroup->buttons().last();
            m_impl->pageButtonGroup->removeButton(btn);
            m_impl->pageButtonLayout->removeWidget(btn);
            btn->hide();
            btn->deleteLater();
        }
    }

    m_impl->currentPage->setRange(m_impl->model->pageCount() ? 1 : 0, m_impl->model->pageCount());
    m_impl->total->setText(QString::number(m_impl->model->pageCount()));
}

void QCtmMultiPageButtonBox::onCurrentPageChanged()
{
    if (!m_impl->model)
        return;
    auto halfRange  = m_impl->pageButtonCount / 2;
    bool even       = !(m_impl->pageButtonCount % 2);
    auto rightCount = std::min(m_impl->model->pageCount() - m_impl->model->currentPage() - 1 + even, halfRange);
    auto leftCount  = std::min(halfRange + (halfRange - rightCount), m_impl->model->currentPage());

    m_impl->pageButtonBeginIndex = m_impl->model->currentPage() - leftCount;
    auto end = m_impl->pageButtonBeginIndex + std::min(m_impl->model->pageCount() - m_impl->pageButtonBeginIndex, m_impl->pageButtonCount);
    m_impl->pageButtonGroup->blockSignals(true);
    for (int i = m_impl->pageButtonBeginIndex; i < end; i++)
    {
        auto btn = m_impl->pageButtonGroup->buttons()[i - m_impl->pageButtonBeginIndex];
        btn->setText(QString::number(i + 1));
        if (i == m_impl->model->currentPage())
        {
            btn->setChecked(true);
        }
    }
    m_impl->pageButtonGroup->blockSignals(false);

    m_impl->currentPage->blockSignals(true);
    m_impl->currentPage->setValue(m_impl->model ? m_impl->model->currentPage() + 1 : 0);
    m_impl->currentPage->blockSignals(false);

    updateState();
}

void QCtmMultiPageButtonBox::onModelReset()
{
    onPageCountChanged();
    onCurrentPageChanged();
}

void QCtmMultiPageButtonBox::updateState()
{
    m_impl->next->setDisabled(m_impl->model && m_impl->model->currentPage() + 1 >= m_impl->model->pageCount());
    m_impl->prev->setEnabled(m_impl->model && m_impl->model->currentPage());
    m_impl->currentPage->setEnabled(m_impl->model);
}
