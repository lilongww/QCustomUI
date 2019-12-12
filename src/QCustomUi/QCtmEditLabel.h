#pragma once

#include "qcustomui_global.h"

#include <QLabel>
#include <QIcon>

class QStyleOptionToolButton;

class QCUSTOMUI_EXPORT QCtmEditLabel : public QLabel
{
	Q_OBJECT

	Q_PROPERTY(bool editable READ editable WRITE setEditable)
	Q_PROPERTY(QIcon editButtonIcon READ editButtonIcon WRITE setEditButtonIcon)
public:
	QCtmEditLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	QCtmEditLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~QCtmEditLabel();

	void setEditable(bool flag);
	bool editable() const;

	void setEditButtonIcon(const QIcon& icon);
	QIcon editButtonIcon() const;

signals:
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
