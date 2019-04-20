#pragma once

#include <QWidget>
#include "ui_StyleWidget.h"

class StyleWidget : public QWidget
{
    Q_OBJECT

public:
    StyleWidget(QWidget *parent = Q_NULLPTR);
    ~StyleWidget();

private:
    Ui::StyleWidget ui;
};
