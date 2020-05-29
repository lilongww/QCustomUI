#pragma once

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QCUSTOMUI_EXPORT QCtmToolBox :public QWidget
{
	Q_OBJECT

public:
	QCtmToolBox(QWidget* parent);
	~QCtmToolBox();

	void addWidget(const QString& title, QWidget* widget);
	void insertWidget(int index, const QString& title, QWidget* widget);
	void removeWidget(QWidget* widget);
	void removeWidget(int index);
	int indexOf(QWidget* widget) const;
	QWidget* widget(int index) const;
	void setStretchFactor(int index, int stretch);
	void setSizes(const QList<int>& sizes);
	void setDefaultStretch(int index, int stretch);
protected:
	void showEvent(QShowEvent *event) override;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};