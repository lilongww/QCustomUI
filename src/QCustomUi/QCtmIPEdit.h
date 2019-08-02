#pragma once

#include "qcustomui_global.h"

#include <QWidget>

#include <memory>

class QStyleOptionFrame;

class QCUSTOMUI_EXPORT QCtmIPEdit : public QWidget
{
	Q_OBJECT

public:
	QCtmIPEdit(QWidget *parent);
	~QCtmIPEdit();

	void setIP(const QString& ip);
	QString ip() const;

protected:
	void paintEvent(QPaintEvent* event) override;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
	void initStyleOption(QStyleOptionFrame* option) const;
private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};
