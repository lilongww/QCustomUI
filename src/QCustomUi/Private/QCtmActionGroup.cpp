#include "QCtmActionGroup_p.h"

#include <QSet>

struct QCtmActionGroup::Impl
{
    QSet<QAction*> actions;
    QAction* current{ nullptr };
};

QCtmActionGroup::QCtmActionGroup(QObject *parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{

}

QCtmActionGroup::~QCtmActionGroup()
{
    
}

void QCtmActionGroup::addAction(QAction* action)
{
    m_impl->actions.insert(action);
    connect(action, &QAction::changed, this, &QCtmActionGroup::actionChanged);
}

void QCtmActionGroup::removeAction(QAction* action)
{
    m_impl->actions.remove(action);
}

void QCtmActionGroup::actionChanged()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action->isChecked()) {
        if (action != m_impl->current) {
            if (m_impl->current)
                m_impl->current->setChecked(false);
            m_impl->current = action;
        }
    }
    else if (action == m_impl->current) {
        m_impl->current = 0;
    }
}
