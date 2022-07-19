#include "MessageTipExample.h"

#include <QCustomUi/QCtmInputDialog.h>
#include <QCustomUi/QCtmMessageBox.h>
#include <QCustomUi/QCtmMessageTipButton.h>
#include <QCustomUi/QCtmMessageTipData.h>
#include <QCustomUi/QCtmMessageTipModel.h>
#include <QCustomUi/QCtmMessageTipView.h>
#include <QCustomUi/QCtmNavigationBar.h>

#include <QWidgetAction>

MessageTipExample::MessageTipExample(QWidget* parent) : QCtmWindow(parent)
{
    setNavigationBar(new QCtmNavigationBar(this));
    auto action    = new QWidgetAction(this);
    auto msgTipBtn = new QCtmMessageTipButton(this);
    action->setDefaultWidget(msgTipBtn);
    navigationBar()->addAction(action, QCtmNavigationBar::ActionPosition::Right);
    auto view = new QCtmMessageTipView(navigationBar());
    view->setTouchControlStyle(true);
    msgTipBtn->setView(view);
    auto model = new QCtmMessageTipModel(this);
    msgTipBtn->setModel(model);
    for (int i = 0; i < 10; i++)
    {
        model->addMessage(
            std::make_shared<QCtmMessageTipData>(QString("Title%1").arg(i + 1), "This is message content.", QDateTime::currentDateTime()));
    }
    connect(view,
            &QCtmMessageTipView::aboutToCloseMessage,
            this,
            [=](auto msg)
            {
                return QCtmMessageBox::Yes == QCtmMessageBox::question(this,
                                                                       tr("Tips"),
                                                                       QString("Delete message:\n%1\n%2\n%3")
                                                                           .arg(msg->title())
                                                                           .arg(msg->content())
                                                                           .arg(msg->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
            });
    connect(view,
            &QCtmMessageTipView::aboutToClearAllMessages,
            this,
            [=] { return QCtmMessageBox::Yes == QCtmMessageBox::question(this, tr("Tips"), QString("Clear all messages?")); });
    connect(view,
            &QCtmMessageTipView::messageClicked,
            this,
            [=](auto msg)
            { return QCtmMessageBox::Yes == QCtmMessageBox::information(this, tr("Tips"), QString("Clicked message:\n%1\n%2\n%3")
                                                                           .arg(msg->title())
                                                                           .arg(msg->content())
                                                                           .arg(msg->dateTime().toString("yyyy-MM-dd hh:mm:ss")));
            });
}

MessageTipExample::~MessageTipExample() {}
