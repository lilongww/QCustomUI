#pragma once

#include "../Attribute.h"

#include <string>

namespace OpenVisa
{
class IOBase
{
public:
    inline IOBase(Object::Attribute const& attr) : m_attr(attr) {}
    virtual ~IOBase() noexcept                         = default;
    virtual void send(const std::string& buffer) const = 0;
    virtual std::string readAll() const                = 0;
    virtual std::string read(size_t size) const        = 0;
    virtual void close() noexcept                      = 0;
    virtual bool connected() const noexcept            = 0;
    virtual size_t avalible() const noexcept           = 0;

protected:
    Object::Attribute const& m_attr;
};
} // namespace OpenVisa
