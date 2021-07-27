#include "MessageTipExample.h"

#include <QCustomUi/QCtmNavigationBar.h>
#include <QCustomUi/QCtmMessageTipButton.h>
#include <QCustomUi/QCtmMessageTipData.h>
#include <QCustomUi/QCtmMessageTipModel.h>
#include <QCustomUi/QCtmMessageTipView.h>

#include <QWidgetAction>

MessageTipExample::MessageTipExample(QWidget* parent)
    : QCtmWindow(parent)
{
    setNavigationBar(new QCtmNavigationBar(this));
    auto action = new QWidgetAction(this);
    auto msgTipBtn = new QCtmMessageTipButton(this);
    action->setDefaultWidget(msgTipBtn);
    navigationBar()->addAction(action, QCtmNavigationBar::ActionPosition::Right);
    msgTipBtn->setView(new QCtmMessageTipView(navigationBar()));
    auto model = new QCtmMessageTipModel(this);
    msgTipBtn->setModel(model);
    model->addMessage(std::make_shared<QCtmMessageTipData>("Title", "This is message content.", QDateTime::currentDateTime()));
}

MessageTipExample::~MessageTipExample()
{

}
