#include "QCtmWidgetItemForMainMenu_p.h"

#include "QCtmSeparator_p.h"
#include "QCtmMainMenuButton_p.h"

#include <QWidgetAction>

struct QCtmWidgetItemForMainMenu::Impl
{
    QAction* action{ nullptr };
    QWidget* widget{ nullptr };
    bool customWidget{ false };
};

QCtmWidgetItemForMainMenu::QCtmWidgetItemForMainMenu(QAction* action, Qt::Orientation orientation, QWidget* parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->action = action;

    auto wa = qobject_cast<QWidgetAction*>(action);
    if (wa)
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
            auto setToolButtonStyle = [](QAction* action, QCtmMainMenuButton* btn)
            {
                if (action->icon().isNull())
                    btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
                else if (action->text().isEmpty())
                    btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
                else
                    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            };
            auto btn = new QCtmMainMenuButton(parent);
            btn->setDefaultAction(action);
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

QCtmWidgetItemForMainMenu::~QCtmWidgetItemForMainMenu()
{
    if (!m_impl->customWidget&&m_impl->widget)
        delete m_impl->widget;
}

void QCtmWidgetItemForMainMenu::release()
{
    m_impl->action = nullptr;
    m_impl->widget = nullptr;
}

QAction* QCtmWidgetItemForMainMenu::action() const
{
    return m_impl->action;
}

QWidget* QCtmWidgetItemForMainMenu::widget() const
{
    return m_impl->widget;
}

bool QCtmWidgetItemForMainMenu::isCustomWidget() const
{
    return m_impl->customWidget;
}
