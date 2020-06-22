#include "QCtmSeparator_p.h"

#include <QPainter>

QCtmSeparator::QCtmSeparator(Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent)
    , m_orientation(orientation)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QCtmSeparator::~QCtmSeparator()
{
}

Qt::Orientation QCtmSeparator::orientation() const
{
    return m_orientation;
}

QSize QCtmSeparator::sizeHint() const
{
    QStyleOption opt;
    initStyleOption(&opt);
    const int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget());
    return QSize(extent, extent);
}

void QCtmSeparator::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QStyleOption opt;
    initStyleOption(&opt);
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget());
}

void QCtmSeparator::initStyleOption(QStyleOption* opt) const
{
    opt->initFrom(this);
    if (orientation() == Qt::Horizontal)
        opt->state |= QStyle::State_Horizontal;
    const auto& rect = style()->subElementRect(QStyle::SE_FrameContents, opt, this);
    if (rect.isValid())
    {
        opt->rect = rect;
    }
}

void QCtmSeparator::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
}
