#pragma once

#include "../Attribute.h"

#include <functional>
#include <stdexcept>

namespace OpenVisa
{
[[noreturn]] inline void visaThrow(const Object::Attribute& attr, std::string_view description)
{
    throw std::runtime_error(std::format("{}:{}", attr.deviceName(), description));
}

template<typename Func>
requires std::is_invocable_v<Func>
inline auto visaReThrow(const Object::Attribute& attr, const Func& func)
{
    if (attr.deviceName().empty())
        return std::invoke(func);
    else
    {
        try
        {
            return std::invoke(func);
        }
        catch (const std::exception& e)
        {
            visaThrow(attr, e.what());
        }
    }
}
} // namespace OpenVisa