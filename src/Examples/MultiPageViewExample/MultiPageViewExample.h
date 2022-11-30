#pragma once

#include "ui_MultiPageViewExample.h"

#include <QCustomUi/QCtmMultiPageButtonBox.h>
#include <QCustomUi/QCtmMultiPageStringListModel.h>
#include <QCustomUi/QCtmWindow.h>

class MultiPageViewExample : public QCtmWindow
{
    Q_OBJECT
public:
    MultiPageViewExample();
    ~MultiPageViewExample();

private:
    Ui::MultiPageViewExample ui;
};