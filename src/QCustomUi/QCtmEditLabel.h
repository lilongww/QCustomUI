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

#include <QLabel>
#include <QIcon>

class QStyleOptionToolButton;

class QCUSTOMUI_EXPORT QCtmEditLabel : public QLabel
{
	Q_OBJECT

	Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
	Q_PROPERTY(QIcon editButtonIcon READ editButtonIcon WRITE setEditButtonIcon)
public:
	QCtmEditLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	QCtmEditLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~QCtmEditLabel();

	void setReadOnly(bool flag);
	bool isReadOnly() const;

	void setEditButtonIcon(const QIcon& icon);
	QIcon editButtonIcon() const;

signals:
	/**
	 * @brief		Edit finished
	 */
	void editFinished();

protected:
	bool event(QEvent* e) override;
	QSize minimumSizeHint() const override;
private:
	void init();

private slots:
	void onEditButtonClicked();
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
