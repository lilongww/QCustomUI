/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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

#include "QCtmHeaderView.h"
#include "Private/qheaderview_p.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionButton>

#include <tuple>

constexpr int checkboxMargin = 5;

struct QCtmHeaderView::Impl
{
    std::vector<std::tuple<bool, Qt::CheckState>> state;
    std::map<int, bool> readOnlyState;
};

/*!
    \class      QCtmHeaderView
    \brief      自定义表头，提供了 checkbox 勾选功能.
    \inherits   QHeaderView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmHeaderView.h
*/

/*!
    \brief      构造函数 \a orientation, \a parent.
*/
QCtmHeaderView::QCtmHeaderView(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent), m_impl(std::make_unique<Impl>())
{
}

/*!
    \brief      析构函数.
*/
QCtmHeaderView::~QCtmHeaderView() {}

/*!
    \reimp
*/
void QCtmHeaderView::setModel(QAbstractItemModel* model)
{
    QHeaderView::setModel(model);
    connect(model, &QAbstractItemModel::columnsInserted, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::rowsInserted, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::columnsMoved, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::rowsMoved, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::columnsRemoved, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &QCtmHeaderView::onModelReset);
    connect(model, &QAbstractItemModel::modelReset, this, &QCtmHeaderView::onModelReset);
    connect(model,
            &QAbstractItemModel::dataChanged,
            this,
            [=]([[maybe_unused]] const QModelIndex& topLeft, [[maybe_unused]] const QModelIndex& bottomRight, const QVector<int>& roles)
            {
                if (roles.contains(Qt::CheckStateRole) || roles.isEmpty())
                {
                    onModelReset();
                }
            });
    onModelReset();
}

/*!
    \brief      设置 \a logicIndex 行/列只读 \a enable.
*/
void QCtmHeaderView::setReadOnly(int logicIndex, bool enable)
{
    if (logicIndex >= m_impl->state.size())
        return;
    m_impl->readOnlyState[logicIndex] = enable;
}

/*!
    \reimp
*/
void QCtmHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    auto [show, state] = m_impl->state[logicalIndex];
    auto boxRect       = doCheckBoxRect(logicalIndex);
    if (!rect.isValid())
        return;
    { // header
#if QT_VERSION_MAJOR < 6
        QStyleOptionHeader opt;
#else
        QStyleOptionHeaderV2 opt;
#endif
        QPointF oldBO = painter->brushOrigin();
        initStyleOption(&opt);
        QBrush oBrushButton = opt.palette.brush(QPalette::Button);
        QBrush oBrushWindow = opt.palette.brush(QPalette::Window);
        initStyleOptionForIndex(&opt, logicalIndex);
        opt.rect            = rect;
        opt.text            = "";
        QBrush nBrushButton = opt.palette.brush(QPalette::Button);
        QBrush nBrushWindow = opt.palette.brush(QPalette::Window);
        if (oBrushButton != nBrushButton || oBrushWindow != nBrushWindow)
        {
            painter->setBrushOrigin(opt.rect.topLeft());
        }
        painter->save();
        style()->drawControl(QStyle::CE_Header, &opt, painter, this);
        painter->restore();
        painter->setBrushOrigin(oldBO);
        if (!model())
            return;
        QTextOption txtOpt(opt.textAlignment);
        txtOpt.setWrapMode(QTextOption::ManualWrap);
        painter->drawText(show ? QRect(boxRect.right(), rect.top(), rect.width() - (boxRect.right() - rect.left()), rect.height()) : rect,
                          this->model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString(),
                          txtOpt);
    }
    if (!show)
        return;
    { // button
        QStyleOptionButton opt;
        opt.rect = boxRect;
        if (state == Qt::Checked)
            opt.state = QStyle::State_On;
        else if (state == Qt::PartiallyChecked)
            opt.state = QStyle::State_NoChange;
        else
            opt.state = QStyle::State_Off;
        opt.state |= QStyle::State_Enabled;
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &opt, painter, this);
    }
}

/*!
    \reimp
*/
void QCtmHeaderView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        const bool isHorizontal = orientation() == Qt::Horizontal;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        auto pos = isHorizontal ? e->x() : e->y();
#else
        auto pos = isHorizontal ? e->position().x() : e->position().y();
#endif
        auto logicalIndex = logicalIndexAt(static_cast<int>(pos));
        if (logicalIndex < 0)
        {
            QHeaderView::mousePressEvent(e);
            return;
        }
        if (auto it = m_impl->readOnlyState.find(logicalIndex); it != m_impl->readOnlyState.end() && it->second)
        {
            QHeaderView::mousePressEvent(e);
            return;
        }

        auto [checkable, state] = m_impl->state[logicalIndex];
        if (!checkable)
        {
            QHeaderView::mousePressEvent(e);
            return;
        }
        if (!model())
            return;
        const auto& rect = doCheckBoxRect(logicalIndex);
        if (rect.contains(e->pos()))
        {
            state = state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
            if (isHorizontal)
            {
                std::function<void(int, const QModelIndex&)> visiter = [&](int count, const QModelIndex& parent)
                {
                    for (int i = 0; i < count; ++i)
                    {
                        const auto& index = model()->index(i, logicalIndex, parent);
                        if (index.isValid())
                        {
                            model()->setData(index, state, Qt::CheckStateRole);
                            if (model()->hasChildren(index))
                            {
                                visiter(model()->rowCount(index), index);
                            }
                        }
                    }
                };

                visiter(model()->rowCount(), QModelIndex());
            }
            else
            {
                for (int i = 0; i < model()->columnCount(); ++i)
                {
                    const auto& index = model()->index(logicalIndex, i);
                    if (index.isValid())
                    {
                        model()->setData(index, state, Qt::CheckStateRole);
                    }
                }
            }
            return;
        }
    }
    QHeaderView::mousePressEvent(e);
}

/*!
    \brief      计算 checkbox 的包围盒 \a logicalIndex.
*/
QRect QCtmHeaderView::doCheckBoxRect(int logicalIndex) const
{
    auto position = sectionViewportPosition(logicalIndex);
    auto size     = sectionSize(logicalIndex);
    QStyleOptionButton opt;
    if (orientation() == Qt::Horizontal)
    {
        opt.rect = QRect(QPoint(position, 0), QSize(size, this->height()));
    }
    else
        opt.rect = QRect(QPoint(0, position), QSize(this->width(), size));
    const auto& rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &opt, this);
    return QRect { rect.x() + checkboxMargin, rect.y(), rect.width(), rect.height() };
}

/*!
    \reimp
*/
QSize QCtmHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    auto size = QHeaderView::sectionSizeFromContents(logicalIndex);
    if (auto [checkable, igonre] = m_impl->state[logicalIndex]; checkable)
    {
        return QSize(size.width() + doCheckBoxRect(logicalIndex).width(), size.height());
    }
    return size;
}

#if QT_VERSION_MAJOR < 6
void QCtmHeaderView::initStyleOptionForIndex(QStyleOptionHeader* option, int logicalIndex) const
{
    Q_D(const QHeaderView);

    auto isSectionSelected = [d](int section)
    {
        int i = section * 2;
        if (i < 0 || i >= d->sectionSelected.size())
            return false;
        if (d->sectionSelected.testBit(i)) // if the value was cached
            return d->sectionSelected.testBit(i + 1);
        bool s = false;
        if (d->orientation == Qt::Horizontal)
            s = d->isColumnSelected(section);
        else
            s = d->isRowSelected(section);
        d->sectionSelected.setBit(i + 1, s); // selection state
        d->sectionSelected.setBit(i, true);  // cache state
        return s;
    };

    if (!option)
        return;
    QStyleOptionHeader& opt = *option;

    QStyle::State state = QStyle::State_None;
    if (window()->isActiveWindow())
        state |= QStyle::State_Active;
    if (d->clickableSections)
    {
        if (logicalIndex == d->hover)
            state |= QStyle::State_MouseOver;
        if (logicalIndex == d->pressed)
            state |= QStyle::State_Sunken;
        else if (d->highlightSelected)
        {
            if (d->sectionIntersectsSelection(logicalIndex))
                state |= QStyle::State_On;
            if (isSectionSelected(logicalIndex))
                state |= QStyle::State_Sunken;
        }
    }
    if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex)
        opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder) ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;

    // setup the style options structure
    QVariant textAlignment = d->model->headerData(logicalIndex, d->orientation, Qt::TextAlignmentRole);
    opt.section            = logicalIndex;
    opt.state |= state;
    opt.textAlignment = textAlignment.isValid() ? textAlignment.value<Qt::Alignment>() : d->defaultAlignment;

    opt.iconAlignment = Qt::AlignVCenter;
    opt.text          = d->model->headerData(logicalIndex, d->orientation, Qt::DisplayRole).toString();

    const QVariant variant = d->model->headerData(logicalIndex, d->orientation, Qt::DecorationRole);
    opt.icon               = qvariant_cast<QIcon>(variant);
    if (opt.icon.isNull())
        opt.icon = qvariant_cast<QPixmap>(variant);

    QVariant var = d->model->headerData(logicalIndex, d->orientation, Qt::FontRole);
    if (var.isValid() && var.canConvert<QFont>())
        opt.fontMetrics = QFontMetrics(qvariant_cast<QFont>(var));

    QVariant foregroundBrush = d->model->headerData(logicalIndex, d->orientation, Qt::ForegroundRole);
    if (foregroundBrush.canConvert<QBrush>())
        opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));

    QVariant backgroundBrush = d->model->headerData(logicalIndex, d->orientation, Qt::BackgroundRole);
    if (backgroundBrush.canConvert<QBrush>())
    {
        opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
        opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
    }

    // the section position
    int visual = visualIndex(logicalIndex);
    Q_ASSERT(visual != -1);

    auto recalcSectionStartPos = [d]()
    {
        int pixelpos = 0;
        for (const QHeaderViewPrivate::SectionItem& i : d->sectionItems)
        {
            i.calculated_startpos = pixelpos; // write into const mutable
            pixelpos += i.size;
        }
        d->sectionStartposRecalc = false;
    };

    auto isFirstVisibleSection = [d, &recalcSectionStartPos](int section)
    {
        if (d->sectionStartposRecalc)
            recalcSectionStartPos();
        const QHeaderViewPrivate::SectionItem& item = d->sectionItems.at(section);
        return item.size > 0 && item.calculated_startpos == 0;
    };

    auto isLastVisibleSection = [d, &recalcSectionStartPos](int section)
    {
        if (d->sectionStartposRecalc)
            recalcSectionStartPos();
        const QHeaderViewPrivate::SectionItem& item = d->sectionItems.at(section);
        return item.size > 0 && item.calculatedEndPos() == d->length;
    };

    bool first = isFirstVisibleSection(visual);
    bool last  = isLastVisibleSection(visual);
    if (first && last)
        opt.position = QStyleOptionHeader::OnlyOneSection;
    else if (first)
        opt.position = d->reverse() ? QStyleOptionHeader::End : QStyleOptionHeader::Beginning;
    else if (last)
        opt.position = d->reverse() ? QStyleOptionHeader::Beginning : QStyleOptionHeader::End;
    else
        opt.position = QStyleOptionHeader::Middle;
    opt.orientation = d->orientation;
    // the selected position
    bool previousSelected = isSectionSelected(this->logicalIndex(visual - 1));
    bool nextSelected     = isSectionSelected(this->logicalIndex(visual + 1));
    if (previousSelected && nextSelected)
        opt.selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
    else if (previousSelected)
        opt.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
    else if (nextSelected)
        opt.selectedPosition = QStyleOptionHeader::NextIsSelected;
    else
        opt.selectedPosition = QStyleOptionHeader::NotAdjacent;
}
#endif

/*!
    \brief      响应 model reset.
*/
void QCtmHeaderView::onModelReset()
{
    if (!model())
        return;
    const bool isHorizontal = orientation() == Qt::Horizontal;
    m_impl->state.resize(model()->columnCount());
    int jCount = isHorizontal ? model()->columnCount() : model()->rowCount();
    for (int j = 0; j < jCount; j++)
    {
        bool checkable { false };
        bool hasChecked { false };
        bool hasUnChecked { false };
        bool hasPartiallyChecked { false };
        int iCount = isHorizontal ? model()->rowCount() : model()->columnCount();
        for (int i = 0; i < iCount; i++)
        {
            auto index = isHorizontal ? model()->index(i, j) : model()->index(j, i);
            if (!checkable)
                checkable = index.flags().testFlag(Qt::ItemIsUserCheckable);
            switch (index.data(Qt::CheckStateRole).value<Qt::CheckState>())
            {
            case Qt::Unchecked:
                hasUnChecked = true;
                break;
            case Qt::PartiallyChecked:
                hasPartiallyChecked = true;
                break;
            case Qt::Checked:
                hasChecked = true;
                break;
            }
        }
        if (hasPartiallyChecked || (hasUnChecked && hasChecked))
            m_impl->state[j] = std::make_tuple(checkable, Qt::PartiallyChecked);
        else if (hasUnChecked)
            m_impl->state[j] = std::make_tuple(checkable, Qt::Unchecked);
        else if (hasChecked)
            m_impl->state[j] = std::make_tuple(checkable, Qt::Checked);
    }
    this->viewport()->update();
}
