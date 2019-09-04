#include "QCtmNavigationSidePanel.h"
#include "QCtmNavigationBar.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>
#include <QStyleOption>
#include <QApplication>
#include <QMouseEvent>

#include <assert.h>

constexpr int margin = 8;

struct QCtmNavigationSidePanel::Impl
{
	DockArea dockArea{ DockArea::Left };
	QAction* bindAction{ nullptr };
	QCtmNavigationBar* navigationBar{ nullptr };
	QScrollArea* area{ nullptr };

	QWidget* titleBar{ nullptr };
	QPushButton* closeBtn{ nullptr };
	QLabel* title{ nullptr };
    bool popup{ true };

	QWidget* initTitleBar(QWidget* parent)
	{
		titleBar = new QWidget(parent);
        titleBar->setObjectName("titleBar");
		QHBoxLayout* layout = new QHBoxLayout(titleBar);
		closeBtn = new QPushButton(titleBar);
		layout->setMargin(0);
		closeBtn->setObjectName("closeBtn");
		title = new QLabel(titleBar);
		title->setObjectName("title");
		layout->addWidget(title, 1);
		layout->addWidget(closeBtn);
		return titleBar;
	}
};

QCtmNavigationSidePanel::QCtmNavigationSidePanel(QCtmNavigationBar *parent)
	: QWidget(parent)
	, m_impl(std::make_unique<Impl>())
{
	setMinimumSize(10, 10);
	m_impl->navigationBar = parent;
	m_impl->area = new QScrollArea(this);
	m_impl->area->setAutoFillBackground(false);
	m_impl->area->setFrameStyle(QFrame::NoFrame);
	m_impl->area->viewport()->setAutoFillBackground(false);
	
	m_impl->area->setWidgetResizable(true);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, margin, 0);
	layout->addWidget(m_impl->initTitleBar(this), 0);
	layout->addWidget(m_impl->area, 1);
	m_impl->titleBar->hide();
    if(parent)
	    parent->window()->installEventFilter(this);

	connect(m_impl->closeBtn, &QPushButton::clicked, this, [=]() { this->close(); });
	this->setAttribute(Qt::WA_TranslucentBackground);
    setPopup(true);
}

QCtmNavigationSidePanel::~QCtmNavigationSidePanel()
{
}

void QCtmNavigationSidePanel::setDockArea(DockArea area)
{
	m_impl->dockArea = area;
	switch (area)
	{
	case DockArea::Left:
		layout()->setContentsMargins(0, 0, margin*2, 0);
		break;
	case DockArea::Right:
		layout()->setContentsMargins(margin, 0, 0, 0);
		break;
	case DockArea::Bottom:
		layout()->setContentsMargins(0, margin, 0, 0);
		break;
	case DockArea::Top:
		layout()->setContentsMargins(0, 0, 0, margin);
		break;
	}
}

QCtmNavigationSidePanel::DockArea QCtmNavigationSidePanel::dockArea() const
{
	return m_impl->dockArea;
}

void QCtmNavigationSidePanel::setWidget(QWidget* widget)
{
	m_impl->area->setWidget(widget);
}

QWidget* QCtmNavigationSidePanel::widget() const
{
	return m_impl->area->widget();
}

QScrollArea* QCtmNavigationSidePanel::viewContainer() const
{
	return m_impl->area;
}

void QCtmNavigationSidePanel::setTitleVisible(bool visible)
{
	m_impl->titleBar->setVisible(visible);
}

bool QCtmNavigationSidePanel::titleVisible() const
{
	return m_impl->titleBar->isVisible();
}

void QCtmNavigationSidePanel::setTitle(const QString& text)
{
	m_impl->title->setText(text);
}

QString QCtmNavigationSidePanel::title() const
{
	return m_impl->title->text();
}

void QCtmNavigationSidePanel::setPopup(bool popup)
{
    m_impl->popup = popup;
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | (popup ? Qt::Popup : Qt::Tool));
}

bool QCtmNavigationSidePanel::popup() const
{
    return m_impl->popup;
}

void QCtmNavigationSidePanel::paintEvent(QPaintEvent *event)
{
	paintShadow(margin);
}

void QCtmNavigationSidePanel::showEvent(QShowEvent *event)
{
	if (!m_impl->navigationBar)
		return;
	auto pos = smartPosition(dockArea());
	move(pos);
	resize(sizeHint());
}

void QCtmNavigationSidePanel::closeEvent(QCloseEvent *event)
{
	emit paneClosed();
}

QSize QCtmNavigationSidePanel::sizeHint() const
{
	if (!m_impl->navigationBar)
		return QWidget::sizeHint();
	return smartSize(m_impl->dockArea);
}

QSize QCtmNavigationSidePanel::smartSize(DockArea area)const
{
	QSize oldSize = QWidget::sizeHint();
    if (!m_impl->navigationBar)
        return oldSize;
	QSize alignSize;
	auto bindActionRect = m_impl->navigationBar->actionRect(m_impl->bindAction);
	auto topWidget = m_impl->navigationBar->window();
	int leftM, rightM, topM, bottomM;
	topWidget->getContentsMargins(&leftM, &topM, &rightM, &bottomM);
	switch (area)
	{
	case DockArea::Left:
		alignSize.setWidth(oldSize.width() > bindActionRect.right() ? oldSize.width() : bindActionRect.right());
		if (alignSize.width() > m_impl->navigationBar->width() / 2)
			alignSize.setWidth(m_impl->navigationBar->width() / 2);
		alignSize.setHeight(topWidget->height() - m_impl->navigationBar->y() - m_impl->navigationBar->height() - bottomM);
		break;
	case DockArea::Right:
		alignSize.setWidth(oldSize.width() > m_impl->navigationBar->width() - bindActionRect.left() ? oldSize.width() : m_impl->navigationBar->width() - bindActionRect.left());
		if (alignSize.width() > m_impl->navigationBar->width() / 2)
			alignSize.setWidth(m_impl->navigationBar->width() / 2);
		alignSize.setHeight(topWidget->height() - m_impl->navigationBar->y() - m_impl->navigationBar->height() - bottomM);
		break;
	case DockArea::Top:
	case DockArea::Bottom:
	{
		alignSize.setWidth(m_impl->navigationBar->width());
		auto h = oldSize.height() > minimumHeight() ? oldSize.height() : minimumHeight();
		alignSize.setHeight(h > topWidget->height() / 2 ? topWidget->height() / 2 : h);
		break;
	}
	}
	return alignSize.boundedTo(maximumSize());
}

void QCtmNavigationSidePanel::setNavigationBar(QCtmNavigationBar* bar)
{
    if (m_impl->navigationBar)
    {
        m_impl->navigationBar->window()->removeEventFilter(this);
    }
    setParent(bar);
    setPopup(m_impl->popup);
    m_impl->navigationBar = bar;
    bar->window()->installEventFilter(this);
}

QCtmNavigationBar* QCtmNavigationSidePanel::navigationBar() const
{
    return m_impl->navigationBar;
}

bool QCtmNavigationSidePanel::eventFilter(QObject* o, QEvent* e)
{
	if (m_impl->navigationBar && o == m_impl->navigationBar->window())
	{
		switch (e->type())
		{
		case QEvent::Move:
		{
			auto pos = smartPosition(dockArea());
			move(pos);
			break;
		}
		case QEvent::Resize:
			resize(sizeHint());
			break;
		}
		
	}
	return false;
}

void QCtmNavigationSidePanel::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if ((windowType() == Qt::Popup)) {
        event->accept();
        QWidget* w;
        while ((w = QApplication::activePopupWidget()) && w != this) {
            w->close();
            if (QApplication::activePopupWidget() == w) // widget does not want to disappear
                w->hide(); // hide at least
        }
        if (!rect().contains(event->pos())) {
            close();
            //this->metaObject()->invokeMethod(this, "close", Qt::QueuedConnection);
        }
    }
}

void QCtmNavigationSidePanel::bindAction(QAction* action)
{
	m_impl->bindAction = action;
}

void QCtmNavigationSidePanel::paintShadow(int shadowWidth)
{
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);
	QLinearGradient line;
	line.setColorAt(0, QColor(0, 0, 0, 50));
	line.setColorAt(1, QColor(0, 0, 0, 0));

	QStyleOption opt;
	opt.initFrom(this);

	switch (m_impl->dockArea)
	{
	case DockArea::Left:
		opt.rect = { 0, 0, this->width() - shadowWidth, this->height() };
		line.setStart(this->width() - shadowWidth, 0);
		line.setFinalStop(this->width(), 0);
		painter.fillRect(this->width() - shadowWidth, 0, shadowWidth, height(), QBrush(line));
		break;
	case DockArea::Right:
		opt.rect = { shadowWidth, 0, this->width() - shadowWidth, this->height() };
		line.setStart(shadowWidth, 0);
		line.setFinalStop(0, 0);
		painter.fillRect(0, 0, shadowWidth, height(), QBrush(line));
		break;
	case DockArea::Top:
		opt.rect = { 0, 0, this->width(), this->height() - shadowWidth };
		line.setStart(0, this->height() - shadowWidth);
		line.setFinalStop(0, this->height());
		painter.fillRect(0, this->height() - shadowWidth, width(), shadowWidth, QBrush(line));
		break;
	case DockArea::Bottom:
		opt.rect = { 0, shadowWidth, this->width(), this->height() - shadowWidth };
		line.setStart(0, shadowWidth);
		line.setFinalStop(0, 0);
		painter.fillRect(0, 0, width(), shadowWidth, QBrush(line));
		break;
	}

	painter.fillRect(opt.rect, opt.palette.background());
}

QPoint QCtmNavigationSidePanel::smartPosition(DockArea area) const
{
    if (!m_impl->navigationBar)
        return QPoint(0, 0);
	auto topWidget = m_impl->navigationBar->window();
	auto pos = topWidget->mapToGlobal(m_impl->navigationBar->pos());
	int leftM, rightM, topM, bottomM;
	topWidget->getContentsMargins(&leftM, &topM, &rightM, &bottomM);
	switch (area)
	{
	case DockArea::Left:
		return{ pos.x(), pos.y() + m_impl->navigationBar->height() };
	case DockArea::Right:
		return{ pos.x() + topWidget->width() - sizeHint().width() - leftM - rightM, pos.y() + m_impl->navigationBar->height() };
	case DockArea::Top:
		return{ pos.x(), pos.y() + m_impl->navigationBar->height() };
	case DockArea::Bottom:
		return{ pos.x(), topWidget->y() + topWidget->height() - sizeHint().height() - bottomM };
	}
	return{};
}
