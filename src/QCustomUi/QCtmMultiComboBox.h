#pragma once

#include "qcustomui_global.h"

#include <QComboBox>

class QCUSTOMUI_EXPORT QCtmMultiComboBox : public QComboBox
{
	Q_OBJECT

public:
	QCtmMultiComboBox(QWidget *parent);
	~QCtmMultiComboBox();

	void setModel(QAbstractItemModel* model);
	QAbstractItemModel* model() const;

	QStringList checkedItems() const;
	QVariantList checkedDatas() const;

	void setChecked(int index, bool checked);
	bool isChecked(int index) const;

private:
	bool eventFilter(QObject *watched, QEvent *event) override;
	void showEvent(QShowEvent *e) override;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
