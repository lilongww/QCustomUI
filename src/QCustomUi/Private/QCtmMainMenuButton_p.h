#pragma once

#include "QCtmToolButton_p.h"

#include <memory>

class QStyleOptionButton;

class QCtmMainMenuButton : public QCtmToolButton
{
	Q_OBJECT

public:
	QCtmMainMenuButton(QWidget *parent);
	~QCtmMainMenuButton();

    void setAlternateColor(const QColor& color);
    const QColor& alternateColor() const;

    void setAlternateEnable(bool enable);
    bool alternateEnable() const;
protected:
	void paintEvent(QPaintEvent*) override;
	void initStyleOption(QStyleOptionButton* opt);
private:
	struct Impl;
	std::shared_ptr<Impl> m_impl;
};
