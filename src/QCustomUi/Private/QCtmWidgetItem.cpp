#include "QCtmWidgetItem_p.h"
#include "QCtmSeparator_p.h"
#include "QCtmToolButton_p.h"

#include <QWidgetAction>

struct QCtmWidgetItem::Impl
{
    QAction* action{ nullptr };
    QWidget* widget{ nullptr };
    bool customWidget{ false };
};

QCtmWidgetItem::QCtmWidgetItem(QAction* action, Qt::Orientation orientation, QWidget* parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->action = action;

    auto wa = qobject_cast<QWidgetAction*>(action);
    if (wa && wa->defaultWidget())
    {
        m_impl->customWidget = true;
        m_impl->widget = wa->defaultWidget();
    }
    else
    {
        if (action->isSeparator())
        {
            m_impl->widget = new QCtmSeparator(orientation, parent);
            m_impl->widget->setObjectName(action->objectName());
            QObject::connect(action, &QObject::objectNameChanged, m_impl->widget, [=](const QString& name) {
                m_impl->widget->setObjectName(name);
                });
        }
        else
        {
            auto setToolButtonStyle = [](QAction* action, QCtmToolButton* btn)
            {
                if (action->icon().isNull())
                    btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
                else if (action->text().isEmpty())
                    btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
                else
                    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            };
            auto btn = new QCtmToolButton(parent);
            btn->setDefaultAction(action);
            btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            btn->setObjectName(action->objectName());
            btn->setVisible(action->isVisible());
            setToolButtonStyle(action, btn);
            QObject::connect(action, &QAction::changed, btn, [=]() {
                setToolButtonStyle(action, btn);
                btn->setVisible(action->isVisible());
                });
            QObject::connect(action, &QObject::objectNameChanged, btn, [=](const QString& name) {btn->setObjectName(name); });
            m_impl->widget = btn;
        }
    }
}

QCtmWidgetItem::~QCtmWidgetItem()
{
    if (!m_impl->customWidget && m_impl->widget)
        delete m_impl->widget;
}

void QCtmWidgetItem::release()
{
    m_impl->action = nullptr;
    m_impl->widget = nullptr;
}

QAction* QCtmWidgetItem::action() const
{
    return m_impl->action;
}

QWidget* QCtmWidgetItem::widget() const
{
    return m_impl->widget;
}

bool QCtmWidgetItem::isCustomWidget() const
{
    return m_impl->customWidget;
}
