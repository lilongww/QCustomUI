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

#pragma once

#include "qcustomui_global.h"

#include "QCtmAbstractMessageTipView.h"

#include <memory>

class QCtmNavigationBar;
using QCtmAbstractMessageTipDataPtr = std::shared_ptr<class QCtmAbstractMessageTipData>;

class QCUSTOMUI_EXPORT QCtmMessageTipView : public QCtmAbstractMessageTipView
{
	Q_OBJECT
		Q_PROPERTY(QColor decoration READ decoration WRITE setDecoration)
		Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
		Q_PROPERTY(QColor timeColor READ timeColor WRITE setTimeColor)
		Q_PROPERTY(QPixmap closeButtonIcon READ closeButtonIcon WRITE setCloseButtonIcon)
public:
	QCtmMessageTipView(QCtmNavigationBar *parent);
	~QCtmMessageTipView();

	virtual void setModel(QCtmAbstractMessageTipModel* model);
	virtual QCtmAbstractMessageTipModel* model()const;
	void setDecoration(const QColor& color);
	const QColor& decoration()const;
	void setTitleColor(const QColor& color);
	const QColor& titleColor()const;
	void setTimeColor(const QColor& color);
	const QColor& timeColor()const;
	void setCloseButtonIcon(const QPixmap& icon);
	const QPixmap& closeButtonIcon()const;
signals:
	void closeButtonClicked(const QModelIndex& index);
	void messageClicked(QCtmAbstractMessageTipDataPtr message);
protected:
	void resizeEvent(QResizeEvent*) override;
	void showEvent(QShowEvent*) override;
	bool eventFilter(QObject* o, QEvent* e) override;

	private slots:
	void onCloseButtonClicked(const QModelIndex& index);
	void onTitleClicked(const QModelIndex& index);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
