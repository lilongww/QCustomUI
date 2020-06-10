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

#include "QCtmHeaderView.h"

#include <QStyleOptionButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include <tuple>

constexpr int checkboxMargin = 5;

struct QCtmHeaderView::Impl
{
	std::vector<std::tuple<bool, Qt::CheckState>> state;
	std::map<int, bool> readOnlyState;
};

QCtmHeaderView::QCtmHeaderView(Qt::Orientation orientation, QWidget *parent)
	: QHeaderView(orientation, parent)
	, m_impl(std::make_unique<Impl>())
{

}

QCtmHeaderView::~QCtmHeaderView()
{
}

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
	connect(model, &QAbstractItemModel::dataChanged, this
		, [=]([[maybe_unused]] const QModelIndex& topLeft
			, [[maybe_unused]] const QModelIndex& bottomRight
			, const QVector<int>& roles)
		{
			if (roles.contains(Qt::CheckStateRole) || roles.isEmpty())
			{
				onModelReset();
			}
		});
	onModelReset();
}

void QCtmHeaderView::setReadOnly(int logicIndex, bool enable)
{
	if (logicIndex >= m_impl->state.size())
		return;
	m_impl->readOnlyState[logicIndex] = enable;
}

void QCtmHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
	painter->save();
	QHeaderView::paintSection(painter, rect, logicalIndex);
	painter->restore();

	auto [show, state] = m_impl->state[logicalIndex];
	if (!show)
		return;

	QStyleOptionButton opt;
	opt.rect = doCheckBoxRect(logicalIndex);
	if (state == Qt::Checked)
		opt.state = QStyle::State_On;
	else if (state == Qt::PartiallyChecked)
		opt.state = QStyle::State_NoChange;
	else
		opt.state = QStyle::State_Off;
	
	opt.state |= QStyle::State_Enabled;

	style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &opt, painter, this);
}

void QCtmHeaderView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		auto pos = orientation() == Qt::Horizontal ? e->x() : e->y();
		auto logicalIndex = logicalIndexAt(pos);
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

		const auto& rect = doCheckBoxRect(logicalIndex);
		if (rect.contains(e->pos()))
		{
			auto count = orientation() == Qt::Horizontal ? model()->rowCount() : model()->columnCount();
			model()->blockSignals(true);
			for (int i = 0; i < count; i++)
			{
				const auto& index = Qt::Horizontal ? model()->index(i, logicalIndex) : model()->index(logicalIndex, i);
				if (index.isValid())
				{
					model()->setData(index, state == Qt::Checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
				}
			}
			model()->blockSignals(false);
			if (Qt::Horizontal == orientation())
				emit model()->dataChanged(model()->index(0, logicalIndex), model()->index(model()->rowCount(), logicalIndex), { Qt::CheckStateRole });
			else
				emit model()->dataChanged(model()->index(logicalIndex, 0), model()->index(logicalIndex, model()->columnCount()), { Qt::CheckStateRole });
			return;
		}
	}
	QHeaderView::mousePressEvent(e);
}

QRect QCtmHeaderView::doCheckBoxRect(int logicalIndex) const
{
	auto position = sectionPosition(logicalIndex);
	auto size = sectionSize(logicalIndex);
	QStyleOptionButton opt;
	if (orientation() == Qt::Horizontal)
	{
		opt.rect = QRect(QPoint(position, 0), QSize(size, this->height()));
	}
	else
		opt.rect = QRect(QPoint(0, position), QSize(this->width(), size));
	const auto& rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &opt, this);
	return QRect{ rect.x() + checkboxMargin, rect.y(), rect.width(), rect.height() };
}

void QCtmHeaderView::onModelReset()
{
	m_impl->state.resize(model()->columnCount());
	int jCount = orientation() == Qt::Horizontal ? model()->columnCount() : model()->rowCount();
	for (int j = 0; j < jCount; j++)
	{
		bool checkable{ false };
		bool hasChecked{ false };
		bool hasUnChecked{ false };
		bool hasPartiallyChecked{ false };
		int iCount = orientation() == Qt::Horizontal ? model()->rowCount() : model()->columnCount();
		for (int i = 0; i < iCount; i++)
		{
			auto index = orientation() == Qt::Horizontal ? model()->index(i, j) : model()->index(j, i);
			if(!checkable)
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
		if(hasPartiallyChecked || (hasUnChecked && hasChecked))
			m_impl->state[j] = std::make_tuple(checkable, Qt::PartiallyChecked);
		else if (hasUnChecked)
			m_impl->state[j] = std::make_tuple(checkable, Qt::Unchecked);
		else if (hasChecked)
			m_impl->state[j] = std::make_tuple(checkable, Qt::Checked);
	}
	this->viewport()->update();
}
