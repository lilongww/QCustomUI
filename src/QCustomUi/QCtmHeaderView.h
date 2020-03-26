#pragma once

#include "qcustomui_global.h"

#include <QHeaderView>

#include <memory>

class QCUSTOMUI_EXPORT QCtmHeaderView : public QHeaderView
{
	Q_OBJECT

public:
	QCtmHeaderView(Qt::Orientation orientation, QWidget *parent);
	~QCtmHeaderView();

	void setModel(QAbstractItemModel* model) override;
	void setReadOnly(int logicIndex, bool enable);
protected:
	void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
	void mousePressEvent(QMouseEvent* e) override;
	QRect doCheckBoxRect(int logicalIndex) const;
private slots:
	void onModelReset();
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
