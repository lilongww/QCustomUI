#include "Attribute.h"

namespace OpenVisa
{
struct Object::Attribute::Impl
{
    std::string termChars { "\n" };
};

/*!
    \class      OpenVisa::Object::Attribute
    \brief      驱动属性集.
    \ingroup    openvisa
    \inmodule   InstrumentDrivers
    \inheaderfile Attribute.h
*/

/*!
    \brief      构造函数.
*/
Object::Attribute::Attribute() : m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
Object::Attribute::~Attribute() {}

/*!
    \brief      设置终结符 \a chars, 默认为 '\n'(0x0A).
    \sa         terminalChars
*/
void Object::Attribute::setTerminalChars(std::string_view chars) { m_impl->termChars = chars; }

/*!
    \brief      返回终结符.
    \sa         setTerminalChars
*/
const std::string& Object::Attribute::terminalChars() const { return m_impl->termChars; }

} // namespace OpenVisa
