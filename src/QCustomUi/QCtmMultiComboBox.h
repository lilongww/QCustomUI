#pragma once

#include "qcustomui_global.h"

#include <QComboBox>

class QCUSTOMUI_EXPORT QCtmMultiComboBox : public QComboBox
{
	Q_OBJECT

public:
	QCtmMultiComboBox(QWidget *parent);
	~QCtmMultiComboBox();

	QAbstractItemModel* model() const;

	QStringList checkedItems() const;
	QVariantList checkedDatas() const;

	void setChecked(int index, bool checked);
	bool isChecked(int index) const;
protected:
	void setModel(QAbstractItemModel* model);

private:
	bool eventFilter(QObject *watched, QEvent *event) override;
	void showEvent(QShowEvent *e) override;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
