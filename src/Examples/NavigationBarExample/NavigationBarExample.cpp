#include "NavigationBarExample.h"

#include <QCustomUi/QCtmNavigationBar.h>

#include <QPainter>
#include <QUrl>
#include <QWidgetAction>
#include <QLineEdit>

NavigationBarExample::NavigationBarExample(QWidget* parent)
    : QCtmWindow(parent)
{
    init();
}

NavigationBarExample::~NavigationBarExample()
{

}

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
    nvBar->addAction(action, QCtmNavigationBar::Right);
    nvBar->addHelp(QUrl("http://llongww.gitee.io/qcustomuidocument"), QIcon(":/QCustomUi/Resources/help-light.png"));

    nvBar->addUser(QIcon(":/QCustomUi/Resources/people-light.png"), "ABC")->setVisible(false);
}
