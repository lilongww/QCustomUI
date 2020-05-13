/*!***************************************************************************
*  Copyright (C) 2019 lilong(DLC)
*  This file is part of the QCustomUi
*  Contact: 634916105@qq.com
*  License: LGPL
*****************************************************************************/
#pragma once

#include "qcustomui_global.h"

#include <QWidget>
#include <QTextLayout>

#include <memory>

class QStyleOptionFrame;

class QCUSTOMUI_EXPORT QCtmIPAddressEdit : public QWidget
{
	Q_OBJECT

public:
	QCtmIPAddressEdit(QWidget *parent);
	~QCtmIPAddressEdit();

	void setIPAddress(const QString& ip);
	QString ipAddress() const;

	void setReadOnly(bool ro);
	bool isReadOnly() const;
signals:
	void editChanged();
	void editFinished();

protected:
	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

private:
	int sectionOfCursorPosition(int position) const;
	QTextLayout& textLayout(int pos) const;
	QRect rectOfIpSection(int section) const;
	QRect boundRect(int section, const QRect& rect) const;
	bool setText(QTextLayout& textLayout, const QString& text);
	int redoTextLayout(QTextLayout& textLayout) const;
	void initStyleOption(QStyleOptionFrame* option) const;
	int xToCursor(int x) const;
	void clearSelection();
	void deleteSelectedText();
	bool hasSelection() const;
	void initActions();
private slots:
	void init();
	void onCustomContextMenuRequested(const QPoint& pos);
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
