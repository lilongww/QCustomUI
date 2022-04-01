#pragma once

#include "Object.h"

namespace OpenVisa
{
class Object::Attribute
{
public:
    Attribute();
    ~Attribute();

    void setTerminalChars(std::string_view chars);
    const std::string& terminalChars() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
} // namespace OpenVisa
