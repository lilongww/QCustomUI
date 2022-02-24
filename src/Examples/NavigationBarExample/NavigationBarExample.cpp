#include "NavigationBarExample.h"

#include <QCustomUi/QCtmNavigationBar.h>

#include <QLineEdit>
#include <QPainter>
#include <QTimer>
#include <QUrl>
#include <QWidgetAction>

NavigationBarExample::NavigationBarExample(QWidget* parent) : QCtmWindow(parent) { init(); }

NavigationBarExample::~NavigationBarExample() {}

void NavigationBarExample::init()
{
    auto nvBar = new QCtmNavigationBar(this);
    nvBar->setFixedHeight(120);
    setNavigationBar(nvBar);

    QPixmap image({ 120, 70 });
    image.fill(QColor(0, 0, 0, 0));
    {
        QPainter p(&image);
        QFont font;
        font.setBold(true);
        font.setPixelSize(20);
        p.setFont(font);
        QTextOption opt;
        opt.setAlignment(Qt::AlignCenter);
        p.drawText(image.rect(), "QCustomUi", opt);
    }
    nvBar->addLogo(image, QCtmNavigationBar::Left);
    nvBar->addAction(tr("Action 1"), QCtmNavigationBar::ActionPosition::Left);

    auto search = new QLineEdit;
    search->addAction(new QAction(QIcon(":/QCustomUi/Resources/search-light.png"), "", this), QLineEdit::ActionPosition::TrailingPosition);
    auto action = new QWidgetAction(this);
    action->setDefaultWidget(search);
    action->setVisible(false);
    nvBar->addAction(action, QCtmNavigationBar::Right);
    nvBar->addHelp(QUrl("http://llongww.gitee.io/qcustomuidocument"), QIcon(":/QCustomUi/Resources/help-light.png"));
    QTimer::singleShot(2000, [=]() { action->setVisible(true); });
    nvBar->addUser(QIcon(":/QCustomUi/Resources/people-light.png"), "ABC")->setVisible(false);
    nvBar->addAction(tr("Center Action"), QCtmNavigationBar::Center);
}
