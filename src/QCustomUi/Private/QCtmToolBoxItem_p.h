#pragma once

#include <QWidget>
#include <QIcon>

#include "ui_QCtmToolBoxItem.h"

#include <memory>

class QCtmToolBoxItem : public QWidget
{
	Q_OBJECT
        Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
public:
	QCtmToolBoxItem(QWidget *parent);
	~QCtmToolBoxItem();

	void setTitle(const QString& text);
	QString title()const;

	void setContent(QWidget* widget);
	QWidget* content()const;

	void setStretch(int stretch);
	int stretch()const;

    void setIcon(const QIcon& icon);
    const QIcon& icon() const;

protected:
	void paintEvent(QPaintEvent *event) override;
    void actionEvent(QActionEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
	Ui::QCtmToolBoxItem ui;
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};