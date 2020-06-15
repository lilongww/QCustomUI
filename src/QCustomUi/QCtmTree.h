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

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QList>
#include <QMouseEvent>
#include <QMap>
#include <QScrollArea>
#include <QSplitter>

#include <memory>

class QCtmTreeItem;

class QCUSTOMUI_EXPORT QCtmTree : public QWidget
{
	Q_OBJECT
        Q_PROPERTY(bool exclusive READ exclusive WRITE setExclusive)
public:
	QCtmTree(QWidget *parent = nullptr);
	~QCtmTree();

    QCtmTreeItem* addWidget(const QString& title, QWidget* widget);
    QCtmTreeItem* insertWidget(int index, const QString& title, QWidget* widget);
	void removeItem(QCtmTreeItem* item);
	int indexOf(QCtmTreeItem* item) const;
	QCtmTreeItem* item(int index) const;
	void setAllExpand(bool expand) const;
	int count() const;
    void setExclusive(bool exclusive);
    bool exclusive() const;
    void setSizes(const QList<int>& sizes);
signals:
    /**
     * @brief		emit this signal when the item's expand state is changed.
     */
    void itemExpandChanged(QCtmTreeItem* item, bool expand);

    /**
     * @brief		emit this signal when the title bar of the item has being clicked.
     */
    void itemTitleClicked(QCtmTreeItem* item, bool expand);
protected:
	bool allClosed()const;
	int total(const QList<int>& sizes) const;
    void showEvent(QShowEvent* event) override;
private slots:
	void childExpandChanged(QCtmTreeItem* item, bool expand, int size);
    void doResize();
    void onItemExpand(bool expand);
    void onItemClicked(bool expand);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

class QCUSTOMUI_EXPORT QCtmTreeItem:public QWidget
{
	Q_OBJECT
public:

	void setWidget(QWidget* widget);
	QWidget* widget() const;
	void setTitle(const QString& title);
	QString title() const;
	bool isExpand()const;
	void setExpand(bool expand);
    QAction* addAction(const QIcon& icon, const QString& text);
    QAction* addAction(const QString& text);
    QAction* insertAction(int index, const QIcon& icon, const QString& text);
    QAction* insertAction(int index, const QString& text);
    void removeAction(QAction *action);
    int count() const;
    int indexOf(QAction* action);
    QAction* actionAt(int index);

signals:
    /**
     * @brief		emit this signal when the item's expand state is changed.
     */
    void expandChanged(bool expand);

    /**
     * @brief		emit this signal when the title bar of the item has being clicked.
     */
    void titleClicked(bool expand);
protected:
    QCtmTreeItem(const QString& title, QCtmTree* parent);
    ~QCtmTreeItem();

	private slots:
	void onClicked();
	void resizeEvent(QResizeEvent *event);

private:
	int suggestSize()const;
    void setSuggestSize(int size);
    void insertAction(int index, QAction* action);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;

	friend QCtmTree;
};
