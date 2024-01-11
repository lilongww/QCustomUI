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
#include "QCtmRecentView.h"
#include "QCtmRecentModel.h"
#include "QCtmRecentViewDelegate.h"

#include <QHeaderView>
#include <QPainter>
#include <QScrollBar>

struct QCtmRecentView::Impl
{
};

/*!
    \class      QCtmRecentView
    \brief      仿vs启动界面最近项目表view.
    \inherits   QTreeView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmRecentView.h
*/

/*!
    \fn         void QCtmRecentView::topButtonClicked(const QModelIndex& index)
    \brief      置顶按钮被点击时发送该消息 \a index.
*/

/*!
    \fn         void QCtmRecentView::recentClicked(const QCtmRecentData& data)
    \brief      最近项目被点击时，发送该消息 \a data.
*/

/*!
    \fn         void QCtmRecentView::recentDoubleClicked(const QCtmRecentData& data)
    \brief      最近项目被双击时，发送该消息 \a data.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmRecentView::QCtmRecentView(QWidget* parent) : QTreeView(parent), m_impl(std::make_unique<Impl>())
{
    auto d = new QCtmRecentViewDelegate(this);
    setItemDelegate(d);
    this->viewport()->setProperty("_interal_name", "qcustomui_qctmrecentview_viewport");
    this->viewport()->installEventFilter(d);
    this->header()->setVisible(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(d, &QCtmRecentViewDelegate::topButtonClicked, this, &QCtmRecentView::topButtonClicked);
    connect(this, &QCtmRecentView::topButtonClicked, this, &QCtmRecentView::onTopButtonClicked);
    connect(this,
            &QAbstractItemView::clicked,
            this,
            [&](const auto& index)
            {
                auto d = model()->dataOfIndex(index);
                if (!d)
                    return;
                emit recentClicked(*d);
            });
    connect(this,
            &QAbstractItemView::doubleClicked,
            this,
            [&](const auto& index)
            {
                auto d = model()->dataOfIndex(index);
                if (!d)
                    return;
                emit recentDoubleClicked(*d);
            });
}

/*!
    \brief      析构函数.
*/
QCtmRecentView::~QCtmRecentView() {}

/*!
    \overload
                设置数据 \a model.
    \sa         model
*/
void QCtmRecentView::setModel(QCtmRecentModel* model) { QTreeView::setModel(model); }

/*!
    \overload
                返回数据model.
    \sa         setModel
*/
QCtmRecentModel* QCtmRecentView::model() const { return qobject_cast<QCtmRecentModel*>(QTreeView::model()); }

/*!
    \brief      设置置顶按钮图标 \a icon.
    \sa         topButtonIcon
*/
void QCtmRecentView::setTopButtonIcon(const QIcon& icon)
{
    itemDelegate()->setProperty("topButtonIcon", icon);
    viewport()->update();
}

/*!
    \brief      返回置顶按钮图标.
    \sa         setTopButtonIcon
*/
QIcon QCtmRecentView::topButtonIcon() const { return itemDelegate()->property("topButtonIcon").value<QIcon>(); }

/*!
    \brief      设置置顶按钮是否可见 \a v.
    \sa         topButtonVisible
*/
void QCtmRecentView::setTopButtonVisible(bool v)
{
    itemDelegate()->setProperty("topButtonVisible", v);
    viewport()->update();
}

/*!
    \brief      返回指定按钮是否可见.
    \sa         setTopButtonVisible
*/
bool QCtmRecentView::topButtonVisible() const { return itemDelegate()->property("topButtonVisible").toBool(); }

/*!
    \reimp
*/
void QCtmRecentView::reset()
{
    QTreeView::reset();
    for (int r = 0; r < model()->rowCount(); ++r)
    {
        setRowHidden(r, {}, !model()->hasChildren(model()->index(r, 0, {})));
    }
    expandAll();
}

/*!
    \brief      响应置顶按钮点击信号，修改项目数据 \a index 的置顶状态.
    \sa         topButtonClicked
*/
void QCtmRecentView::onTopButtonClicked(const QModelIndex& index)
{
    model()->setData(index, !index.data(QCtmRecentModel::Roles::isTop).toBool(), QCtmRecentModel::Roles::isTop);
}
