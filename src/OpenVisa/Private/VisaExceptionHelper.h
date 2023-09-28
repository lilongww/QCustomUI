/*********************************************************************************
**                                                                              **
**  Copyright (C) 2022-2023 LiLong                                              **
**  This file is part of OpenVisa.                                              **
**                                                                              **
**  OpenVisa is free software: you can redistribute it and/or modify            **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  OpenVisa is distributed in the hope that it will be useful,                 **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with OpenVisa.  If not, see <https://www.gnu.org/licenses/>.          **
**********************************************************************************/
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