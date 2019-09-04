#include "QCtmLogEvent.h"

struct QCtmLogEvent::Impl
{
    QCtmLogDataPtr log;
};

QCtmLogEvent::QCtmLogEvent(QCtmLogDataPtr log)
    : QEvent((QEvent::Type)QCtmLog)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->log = log;
}

QCtmLogEvent::~QCtmLogEvent()
{

}

QCtmLogDataPtr QCtmLogEvent::log() const
{
    return m_impl->log;
}
