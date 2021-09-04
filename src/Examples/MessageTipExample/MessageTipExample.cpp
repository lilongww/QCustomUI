#include "MessageTipExample.h"

#include <QCustomUi/QCtmNavigationBar.h>
#include <QCustomUi/QCtmMessageTipButton.h>
#include <QCustomUi/QCtmMessageTipData.h>
#include <QCustomUi/QCtmMessageTipModel.h>
#include <QCustomUi/QCtmMessageTipView.h>
#include <QCustomUi/QCtmInputDialog.h>

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
    for (int i = 0; i < 10; i++)
    {
        model->addMessage(std::make_shared<QCtmMessageTipData>(QString("Title%1").arg(i + 1)
            , "This is message content."
            , QDateTime::currentDateTime()));
    }
}

MessageTipExample::~MessageTipExample()
{

}
