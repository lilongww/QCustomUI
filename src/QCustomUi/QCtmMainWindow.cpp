#include "QCtmMainWindow.h"
#include "QCtmNavigationSideBar.h"
#include "QCtmTabWidget.h"
#include "QCtmToolBox.h"
#include "ui_QCtmMainWindow.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QResizeEvent>

#include <assert.h>
struct QCtmMainWindow::Impl
{
	QCtmNavigationSideBar* mainSideBar{ nullptr };
	QWidget* subSideBar{ nullptr };
	SideBarArea sideBarArea{ SideBarArea::Left };
	MessageBarArea messageBarArea{ MessageBarArea::Bottom };

	QWidget* content{ nullptr };
	QCtmTabWidget* messageBar{ nullptr };
	QWidget* propertyBar{ nullptr };

};

QCtmMainWindow::QCtmMainWindow(QWidget *parent)
	: QCtmWindow(parent)
	, ui(new Ui::QCtmMainWindow)
	, m_impl(std::make_unique<Impl>())
{
	ui->setupUi(this->centralWidget());

	ui->mainSideBarBase->hide();
	ui->messageBarRightBase->hide();
	ui->messageBarBottomBase->hide();
	ui->propertyBase->hide();
	ui->subSideBarBase->hide();
	ui->mainHSplitter->setStretchFactor(2, 1);
	ui->vSplitter->setStretchFactor(0, 1);
	setSideBarArea(SideBarArea::Left);
	ui->contentBase->installEventFilter(this);
    setMessageBarHeight(250);
    setSubSideBarWidth(250);
    setPropertyBarWidth(250);
}

QCtmMainWindow::~QCtmMainWindow()
{
	delete ui;
}

void QCtmMainWindow::setNavigationSideBar(QCtmNavigationSideBar* mainSideBar)
{
	removeNavigationSideBar();
	m_impl->mainSideBar = mainSideBar;
	ui->mainSideBarLayout->addWidget(mainSideBar);
	ui->mainSideBarBase->show();
    mainSideBar->installEventFilter(this);
}

QCtmNavigationSideBar* QCtmMainWindow::navigationSideBar() const
{
	return m_impl->mainSideBar;
}

QCtmNavigationSideBar* QCtmMainWindow::takeNavigationSideBar()
{
    if (m_impl->mainSideBar)
    {
        ui->mainSideBarLayout->takeAt(0);
        ui->mainSideBarBase->hide();
        m_impl->mainSideBar->hide();
        m_impl->mainSideBar->removeEventFilter(this);
        m_impl->mainSideBar->setParent(nullptr);
    }
    auto temp = m_impl->mainSideBar;
    m_impl->mainSideBar = nullptr;
    return temp;
}

void QCtmMainWindow::removeNavigationSideBar()
{
	if (m_impl->mainSideBar)
	{
		ui->mainSideBarBase->hide();
		delete m_impl->mainSideBar;
		m_impl->mainSideBar = nullptr;
	}
}

void QCtmMainWindow::setSubSideBar(QWidget* subSideBar)
{
	removeSubSideBar();
	m_impl->subSideBar = subSideBar;
	ui->subSideBarLayout->addWidget(subSideBar);
	ui->subSideBarBase->show();
    subSideBar->installEventFilter(this);
}

QWidget* QCtmMainWindow::subSideBar() const
{
	return m_impl->subSideBar;
}

QWidget* QCtmMainWindow::takeSubSideBar()
{
    if (m_impl->subSideBar)
    {
        ui->subSideBarLayout->takeAt(0);
        ui->subSideBarBase->hide();
        m_impl->subSideBar->setParent(nullptr);
        m_impl->subSideBar->removeEventFilter(this);
        m_impl->subSideBar->hide();
    }
    auto temp = m_impl->subSideBar;
    m_impl->subSideBar = nullptr;
    return temp;
}

void QCtmMainWindow::removeSubSideBar()
{
	if (m_impl->subSideBar)
	{
		delete m_impl->subSideBar;
		m_impl->subSideBar = nullptr;
		ui->subSideBarBase->hide();
	}
}

void QCtmMainWindow::setContentWidget(QWidget* widget)
{
	if (m_impl->content)
		delete m_impl->content;
	m_impl->content = widget;
	ui->contentLayout->addWidget(widget);
    ui->contentBase->show();
    widget->installEventFilter(this);
}

QWidget* QCtmMainWindow::contentWidget() const
{
	return m_impl->content;
}

QWidget* QCtmMainWindow::takeContentWidget()
{
    if (m_impl->content)
    {
        ui->contentLayout->takeAt(0);
        m_impl->content->hide();
        m_impl->content->removeEventFilter(this);
        m_impl->content->setParent(nullptr);
    }
    auto temp = m_impl->content;
    m_impl->content = nullptr;
    return temp;
}

void QCtmMainWindow::setMessageBar(QCtmTabWidget* messageBar)
{
	if (m_impl->messageBar == messageBar)
		return;
	removeMessageBar();
	m_impl->messageBar = messageBar;
	connect(messageBar, &QCtmTabWidget::areaButtonClicked, this, &QCtmMainWindow::onTabWidgetAreaButtonClicked, Qt::UniqueConnection);
	if(m_impl->messageBarArea == MessageBarArea::Bottom)
	{
		ui->messageBarBottomLayout->addWidget(messageBar);
		ui->messageBarBottomBase->show();
	}
	else
	{
		ui->messageBarRightLayout->addWidget(messageBar);
		ui->messageBarRightBase->show();
	}
    messageBar->installEventFilter(this);
}

QCtmTabWidget* QCtmMainWindow::messageBar() const
{
	return m_impl->messageBar;
}

QCtmTabWidget* QCtmMainWindow::takeMessageBar()
{
    if (m_impl->messageBar)
    {
        switch (m_impl->messageBarArea)
        {
        case MessageBarArea::Bottom:
            ui->messageBarBottomLayout->takeAt(0);
            ui->messageBarBottomBase->hide();
            break;
        case MessageBarArea::Right:
            ui->messageBarRightLayout->takeAt(0);
            ui->messageBarRightBase->hide();
            break;
        }
        m_impl->messageBar->removeEventFilter(this);
        m_impl->messageBar->hide();
        m_impl->messageBar->setParent(nullptr);
    }
    auto temp = m_impl->messageBar;
    m_impl->messageBar = nullptr;
    return temp;
}

void QCtmMainWindow::removeMessageBar()
{
	if (m_impl->messageBar)
	{
		delete m_impl->messageBar;
		m_impl->messageBar = nullptr;
	}

	if (m_impl->messageBarArea == MessageBarArea::Bottom)
		ui->messageBarBottomBase->hide();
	else
		ui->messageBarRightBase->hide();
}

void QCtmMainWindow::setPropertyBar(QWidget* widget)
{
	removePropertyBar();
	m_impl->propertyBar = widget;
	ui->propertyLayout->addWidget(widget);
	ui->propertyBase->show();
    widget->installEventFilter(this);
    if(isVisible())
        doSize();
}

QWidget* QCtmMainWindow::propertyBar() const
{
	return m_impl->propertyBar;
}

QWidget* QCtmMainWindow::takePropertyBar()
{
    if (m_impl->propertyBar)
    {
        ui->propertyLayout->takeAt(0);
        ui->propertyBase->hide();
        m_impl->propertyBar->removeEventFilter(this);
        m_impl->propertyBar->hide();
        m_impl->propertyBar->setParent(nullptr);
    }
    auto temp = m_impl->propertyBar;
    m_impl->propertyBar = nullptr;
    return temp;
}

void QCtmMainWindow::removePropertyBar()
{
	if (m_impl->propertyBar)
	{
		delete m_impl->propertyBar;
		m_impl->propertyBar = nullptr;
		ui->propertyBase->hide();
	}
}

void QCtmMainWindow::setSideBarArea(SideBarArea area)
{
	m_impl->sideBarArea = area;
	switch (area)
	{
	case SideBarArea::Left:
		ui->mainHSplitter->handle(ui->mainHSplitter->count() - 1)->setDisabled(false);
		ui->mainHSplitter->insertWidget(0, ui->mainSideBarBase);
		ui->mainHSplitter->insertWidget(1, ui->subSideBarBase);
		ui->mainHSplitter->handle(1)->setDisabled(true);
		if (m_impl->subSideBar)
		{
			ui->mainHSplitter->handle(2)->setDisabled(ui->subSideBarBase->isHidden());
		}
		break;
	case SideBarArea::Right:
		ui->mainHSplitter->handle(1)->setDisabled(false);
		ui->mainHSplitter->handle(2)->setDisabled(false);
		ui->mainHSplitter->insertWidget(ui->mainHSplitter->count() - 1, ui->mainSideBarBase);
		ui->mainHSplitter->insertWidget(ui->mainHSplitter->count() - 2, ui->subSideBarBase);
		ui->mainHSplitter->handle(ui->mainHSplitter->count()-1)->setDisabled(true);
		break;
	}
}

QCtmMainWindow::SideBarArea QCtmMainWindow::sideBarArea() const
{
	return m_impl->sideBarArea;
}

void QCtmMainWindow::setMessageBarArea(MessageBarArea area)
{
    if (m_impl->messageBarArea == area)
        return;
    m_impl->messageBarArea = area;
    switch (m_impl->messageBarArea)
    {
    case MessageBarArea::Right:
        ui->messageBarBottomBase->hide();
        ui->messageBarBottomLayout->takeAt(0);
        ui->messageBarRightLayout->addWidget(m_impl->messageBar);
        ui->messageBarRightBase->show();
        break;
    case MessageBarArea::Bottom:
        ui->messageBarRightBase->hide();
        ui->messageBarRightLayout->takeAt(0);
        ui->messageBarBottomLayout->addWidget(m_impl->messageBar);
        ui->messageBarBottomBase->show();
        break;
    }
}

QCtmMainWindow::MessageBarArea QCtmMainWindow::messageBarArea() const
{
    return m_impl->messageBarArea;
}

void QCtmMainWindow::setSubSideBarWidth(int width)
{
    ui->subSideBarBase->resize(width, ui->subSideBarBase->height());
	if (isVisible())
	{
        doSize();
	}
}

int QCtmMainWindow::subSideBarWidth() const
{
	return m_impl->subSideBar->width();
}

void QCtmMainWindow::setPropertyBarWidth(int width)
{
    ui->propertyBase->resize(width, ui->propertyBase->height());
	if (isVisible())
	{
        doSize();
	}
}

int QCtmMainWindow::propertyBarWidth() const
{
	return ui->propertyBase->width();
}

void QCtmMainWindow::setMessageBarHeight(int height)
{
    ui->messageBarBottomBase->resize(ui->messageBarBottomBase->width(), height);
    ui->messageBarRightBase->resize(height, ui->messageBarRightBase->height());
    if (isVisible())
    {
        doSize();
    }
}

int QCtmMainWindow::messageBarHeight() const
{
	return ui->messageBarBottomBase->height();
}

bool QCtmMainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui->contentBase&& event->type() == QEvent::Paint)
	{
		QStyleOption opt;
		opt.initFrom(ui->contentBase);
		QPainter p(ui->contentBase);
		ui->contentBase->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, ui->contentBase);
	}
    if (event->type() == QEvent::ShowToParent || event->type() == QEvent::HideToParent)
    {
        bool show = event->type() == QEvent::ShowToParent;
        if (watched == m_impl->content)
        {
            ui->contentBase->setVisible(show);
        }
        else if (watched == m_impl->mainSideBar)
        {
            ui->mainSideBarBase->setVisible(show);
        }
        else if (watched == m_impl->subSideBar)
        {
            ui->subSideBarBase->setVisible(show);
        }
        else if (watched == m_impl->messageBar)
        {
            switch (m_impl->messageBarArea)
            {
            case MessageBarArea::Bottom:
                ui->messageBarBottomBase->setVisible(show);
                break;
            case MessageBarArea::Right:
                ui->messageBarRightBase->setVisible(show);
                break;
            }
        }
        else if (watched == m_impl->propertyBar)
        {
            ui->propertyBase->setVisible(show);
        }
    }
	return QCtmWindow::eventFilter(watched, event);
}

void QCtmMainWindow::showEvent(QShowEvent* e)
{
	QCtmWindow::showEvent(e);
	doSize();
}

void QCtmMainWindow::doSize()
{
	{//mainHSplitter
		QList<int> sizes;
		switch (m_impl->sideBarArea)
		{
		case SideBarArea::Left:
            sizes << ui->mainSideBarBase->width()
                << ui->subSideBarBase->width()
                << this->width() - ui->subSideBarBase->width() - ui->messageBarRightBase->width()
                << ui->messageBarRightBase->width();
			break;
		case SideBarArea::Right:
            sizes << this->width() - ui->subSideBarBase->width() - ui->messageBarRightBase->width()
                << ui->messageBarRightBase->width()
                << ui->subSideBarBase->width()
                << ui->mainSideBarBase->width();
			break;
		}
		ui->mainHSplitter->setSizes(sizes);
	}

	{//vSplitter
        QList<int> sizes;
        sizes << ui->vSplitter->height() - ui->messageBarBottomBase->height()
            << ui->messageBarBottomBase->height();
		ui->vSplitter->setSizes(sizes);
	}

    {//subHSplitter
        QList<int> sizes;
        sizes << ui->subHSplitter->width() - ui->propertyBase->width()
            << ui->propertyBase->width();
        ui->subHSplitter->setSizes(sizes);
    }
}

void QCtmMainWindow::onTabWidgetAreaButtonClicked()
{
    setMessageBarArea(m_impl->messageBarArea == MessageBarArea::Right 
        ? MessageBarArea::Bottom : MessageBarArea::Right);
}

QList<int>& QCtmMainWindow::swap(QList<int>& sizes)
{
	int num = sizes.count() / 2;
	for (int i = 0; i < num; i++)
	{
		if (i != sizes.count() - 1 - i)
			sizes.swap(i, sizes.count() - 1 - i);
	}
	return sizes;
}
