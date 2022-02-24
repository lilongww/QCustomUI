#include "NavigationPane.h"

#include <QCustomUi/QCtmNavigationBar.h>
#include <QCustomUi/QCtmNavigationSidePane.h>

#include <QLabel>

NavigationPane::NavigationPane(QWidget* parent) : QCtmWindow(parent) { init(); }

NavigationPane::~NavigationPane() {}

void NavigationPane::init()
{
    auto nvBar = new QCtmNavigationBar(this);
    setNavigationBar(nvBar);

    auto creatPane = [=](QCtmNavigationSidePane::DockArea area, const QString& text)
    {
        auto bar = new QCtmNavigationSidePane(area, nvBar);
        bar->setWidget(new QLabel(text));
        return bar;
    };

    nvBar->addPane(tr("Left"), QCtmNavigationBar::ActionPosition::Left, creatPane(QCtmNavigationSidePane::DockArea::Left, "Left"));
    nvBar->addPane(tr("Top"), QCtmNavigationBar::ActionPosition::Left, creatPane(QCtmNavigationSidePane::DockArea::Top, "Top"));
    nvBar->addPane(tr("Right"), QCtmNavigationBar::ActionPosition::Right, creatPane(QCtmNavigationSidePane::DockArea::Right, "Right"));
    nvBar->addPane(tr("Bottom"), QCtmNavigationBar::ActionPosition::Left, creatPane(QCtmNavigationSidePane::DockArea::Bottom, "Bottom"));
}
