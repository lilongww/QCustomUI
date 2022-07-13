#include "SwitchButtonExample.h"

#include <QCustomUi/QCtmSwitchButton.h>

#include <QHBoxLayout>

SwitchButtonExample::SwitchButtonExample(QWidget* parent) : QCtmWindow(parent)
{
    auto btn            = new QCtmSwitchButton(this);
    QHBoxLayout* layout = new QHBoxLayout(this->centralWidget());
    layout->addStretch(1);
    layout->addWidget(btn);
    layout->addStretch(1);
    this->setStyleSheet(".SwitchButtonExample{background:white;}");
    // btn->setStyleSheet("QCtmSwitchButton"
    //                    "{"
    //                    "qproperty-uncheckedBackground: #F1ADB3;"
    //                    "qproperty-uncheckedBackgroundBorderColor: transparent;"
    //                    "qproperty-uncheckedHandleBrush: #DB3340;"
    //                    "qproperty-uncheckedHandleBorderColor: transparent;"
    //                    "qproperty-checkedBackground: #ABE3B8;"
    //                    "qproperty-checkedBackgroundBorderColor: transparent;"
    //                    "qproperty-checkedHandleBrush: #2DB84D;"
    //                    "qproperty-checkedHandleBorderColor: transparent;"
    //                    "}");
    //  btn->setChecked(true);
    //  btn->setEnabled(false);
}

SwitchButtonExample::~SwitchButtonExample() {}
