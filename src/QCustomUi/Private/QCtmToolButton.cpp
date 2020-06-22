#include "QCtmToolButton_p.h"

#include <QAction>
#include <QStyle>
#include <QEvent>

struct QCtmToolButton::Impl
{
    bool showToolTip{ true };
};

QCtmToolButton::QCtmToolButton(QWidget* parent)
    : QToolButton(parent)
    , m_impl(std::make_unique<Impl>())
{
    connect(this, &QAbstractButton::toggled, this, [=](bool) {style()->unpolish(this); style()->polish(this); });
}

QCtmToolButton::~QCtmToolButton()
{
}

void QCtmToolButton::setIcon(const QIcon& icon)
{
    if (defaultAction())
    {
        defaultAction()->setIcon(icon);
    }
    else
        QToolButton::setIcon(icon);
}

void QCtmToolButton::setShowToolTips(bool show)
{
    m_impl->showToolTip = show;
}

bool QCtmToolButton::showToolTips() const
{
    return m_impl->showToolTip;
}

bool QCtmToolButton::event(QEvent* e)
{
    if (e->type() == QEvent::ToolTip)
    {
        return m_impl->showToolTip ? QToolButton::event(e) : true;
    }
    return QToolButton::event(e);
}
