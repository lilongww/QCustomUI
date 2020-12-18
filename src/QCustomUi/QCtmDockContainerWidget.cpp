#include "QCtmDockContainerWidget.h"
#include "QCtmDockAreaWidget.h"
#include "Private/QCtmDockSplitter_p.h"

#include <QSplitter>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QDebug>

struct QCtmDockContainerWidget::Impl
{
    QCtmDockSplitter* rootSplitter{ nullptr };
};

/*!
    \class      QCtmDockContainerWidget
    \brief      停靠容器窗口，用于窗口切分.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmDockContainerWidget.h
*/

/*!
    \enum       QCtmDockContainerWidget::DockArea
                停靠区域
    \value      Left
                停靠在目标区域窗口的左侧，目标停靠区域窗口不能为空.
    \value      Top
                停靠在目标区域窗口的顶部，目标停靠区域窗口不能为空.
    \value      Right
                停靠在目标区域窗口的右侧，目标停靠区域窗口不能为空.
    \value      Bottom
                停靠在目标区域窗口的底部，目标停靠区域窗口不能为空.
    \value      Center
                与目标区域窗口合并，如果目标区域窗口为空并且停靠容器中也没有停靠区域窗口，则直接将目标区域窗口添加进停靠容器中.
*/

/*!
    \brief      构造一个父窗口为 \a parent 的停靠容器对象.
*/
QCtmDockContainerWidget::QCtmDockContainerWidget(QWidget* parent)
    : QWidget(parent)
    , m_impl(std::make_unique<Impl>())
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_impl->rootSplitter = new QCtmDockSplitter(this));
    layout->setMargin(0);
}

/*!
    \brief      销毁该停靠容器对象.
*/
QCtmDockContainerWidget::~QCtmDockContainerWidget()
{
}

/*!
    \brief      向该停靠容器窗口的目标区域窗口 \a targetAreaWidget 的停靠区域 \a area 添加一个停靠区域窗口 \a areaWidget.
*/
void QCtmDockContainerWidget::addDockAreaWidget(DockArea area, QCtmDockAreaWidget* areaWidget, QCtmDockAreaWidget* targetAreaWidget)
{
    if (targetAreaWidget && !m_impl->rootSplitter->contains(targetAreaWidget))
    {
        qCritical() << "targetAreaWidget is not a child of this container";
        return;
    }
    switch (area)
    {
    case QCtmDockContainerWidget::None:
        return;
    case QCtmDockContainerWidget::Left:
    case QCtmDockContainerWidget::Top:
    case QCtmDockContainerWidget::Right:
    case QCtmDockContainerWidget::Bottom:
        break;
    case QCtmDockContainerWidget::Center:
        if (!targetAreaWidget && m_impl->rootSplitter->count())
        {
            m_impl->rootSplitter->addWidget(areaWidget);
        }
        break;
    default:
        return;
    }
}
