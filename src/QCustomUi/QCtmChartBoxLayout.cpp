﻿/*********************************************************************************
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
#include "QCtmChartBoxLayout.h"

struct LayoutItem
{
    inline LayoutItem(QCtmChartItem* _item, int _stretch) : item(_item), stretch(_stretch) {}
    QCtmChartItem* item { nullptr };
    int size { 0 };
    bool finished { false };
    int stretch { 0 };
};

struct QCtmChartBoxLayout::Impl
{
    Qt::Orientation orientation;
    std::vector<LayoutItem> items;
    int spacing { 0 };
    QMargins margins { 0, 0, 0, 0 };
    inline Impl(Qt::Orientation o) : orientation(o) {}
};

QCtmChartBoxLayout::QCtmChartBoxLayout(Qt::Orientation orientation, QCtmAbstractChartView* parent)
    : QCtmChartLayout(parent), m_impl(std::make_unique<Impl>(orientation))
{
}

QCtmChartBoxLayout::~QCtmChartBoxLayout() {}

Qt::Orientation QCtmChartBoxLayout::orientation() const { return m_impl->orientation; }

void QCtmChartBoxLayout::addItem(QCtmChartItem* item, int stretch) { m_impl->items.push_back({ item, stretch }); }

void QCtmChartBoxLayout::removeItem(QCtmChartItem* item)
{
    if (auto it = std::find_if(m_impl->items.begin(), m_impl->items.end(), [&item](const auto& li) { return li.item == item; });
        it != m_impl->items.end())
    {
        m_impl->items.erase(it);
    }
}

QCtmChartItem* QCtmChartBoxLayout::itemAt(int index) const { return m_impl->items.at(index).item; }

void QCtmChartBoxLayout::setStretch(int index, int stretch) { m_impl->items[index].stretch = stretch; }

size_t QCtmChartBoxLayout::count() const { return m_impl->items.size(); }

void QCtmChartBoxLayout::setSpacing(int spacing) { m_impl->spacing = spacing; }

int QCtmChartBoxLayout::spacing() const { return m_impl->spacing; }

void QCtmChartBoxLayout::setMargin(const QMargins& margin) { m_impl->margins = margin; }

const QMargins& QCtmChartBoxLayout::margin() const { return m_impl->margins; }

void QCtmChartBoxLayout::calcSize(const QSize& size)
{
    if (m_impl->items.empty())
        return;
    bool hori    = m_impl->orientation == Qt::Horizontal;
    double total = (hori ? size.width() : size.height()) - (m_impl->items.size() - 1) * m_impl->spacing -
                   (hori ? m_impl->margins.left() + m_impl->margins.right() : m_impl->margins.top() + m_impl->margins.bottom());
    // 分配最小
    std::vector<std::reference_wrapper<LayoutItem>> itemRefs;
    std::for_each(m_impl->items.begin(),
                  m_impl->items.end(),
                  [&](auto& li)
                  {
                      auto minSize = hori ? li.item->minimumSize().width() : li.item->minimumSize().height();
                      auto minHint = hori ? li.item->minimumSizeHint().width() : li.item->minimumSizeHint().height();
                      auto size    = minSize ? std::min(minSize, minHint) : minHint;
                      // 分配 fixed
                      if (size >= (hori ? li.item->maximumSize().width() : li.item->maximumSize().height()))
                      {
                          li.finished = true;
                          li.size     = size;
                      }
                      else
                          itemRefs.push_back(li);
                  });
    auto totalStretch = std::accumulate(itemRefs.begin(), itemRefs.end(), 0.0, [](auto a, auto b) { return a + b.get().stretch; });
    auto alloced      = std::accumulate(m_impl->items.begin(), m_impl->items.end(), 0.0, [](auto a, const auto& b) { return a + b.size; });

    while (!itemRefs.empty() && alloced < total)
    {
        bool hasFinished { false };
        // 先分配有stretch的
        if (totalStretch)
        {
            // 分配比例大小
            for (auto& item : itemRefs)
            {
                if (item.get().stretch)
                {
                    auto size    = (total - alloced) * (item.get().stretch / totalStretch);
                    auto maxSize = hori ? item.get().item->maximumSize().width() : item.get().item->maximumSize().height();
                    if (maxSize <= size)
                    {
                        item.get().size     = maxSize;
                        item.get().finished = true;
                        hasFinished         = true;
                        alloced -= item.get().size;
                        totalStretch -= item.get().stretch;
                    }
                }
            }
        }
        else
        {
            if (alloced <= 0)
                break;
            // 均分
            auto avarge = (total - alloced) / itemRefs.size();
            for (auto& item : itemRefs)
            {
                auto size = hori ? item.get().item->maximumSize().width() : item.get().item->maximumSize().height();
                if (avarge >= size)
                {
                    item.get().size     = size;
                    item.get().finished = true;
                    hasFinished         = true;
                    alloced -= size;
                }
            }
        }

        if (hasFinished)
        {
            std::erase_if(itemRefs, [](auto item) { return item.get().finished; });
        }
        else
            break;
    }

    auto offset = hori ? m_impl->margins.left() : m_impl->margins.top();
    for (auto it = m_impl->items.begin(); it != m_impl->items.end(); it++)
    {
        auto& item = *it;
        if (it != m_impl->items.rbegin().base())
        {
            item.item->setGeometry(QRect(hori ? offset : m_impl->margins.left(),
                                         hori ? m_impl->margins.top() : offset,
                                         hori ? item.size : this->geometry().width() - m_impl->margins.left() - m_impl->margins.right(),
                                         hori ? this->geometry().height() - m_impl->margins.top() - m_impl->margins.bottom() : item.size));
        }
        else
        {
            item.item->setGeometry(
                QRect(hori ? offset : m_impl->margins.left(),
                      hori ? m_impl->margins.top() : offset,
                      hori ? total - offset : this->geometry().width() - m_impl->margins.left() - m_impl->margins.right(),
                      hori ? this->geometry().height() - m_impl->margins.top() - m_impl->margins.bottom() : total - offset));
        }
        offset += item.size + m_impl->spacing;
    }
}