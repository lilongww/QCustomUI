/*********************************************************************************
**                                                                              **
**  Copyright (C) 2022 LiLong                                                   **
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

#include "Object.h"

namespace OpenVisa
{
class ObjectAdapter
{
public:
    inline ObjectAdapter(Object& object) {}
    virtual ~ObjectAdapter() {}

    Object& object() { return m_object; }
    template<typename... Args>
    inline void send(std::string_view fmt, const Args&... args)
    {
        m_object.send(fmt, std::forward<const Args&>(args)...);
    }
    inline void sendBlockData(const std::string& data) { m_object.sendBlockData(data); }
    inline std::string readAll() { return m_object.readAll(); }
    inline std::tuple<std::string, bool> read(unsigned long blockSize) { return m_object.read(blockSize); }
    template<typename... Args>
    inline std::string query(std::string_view fmt, const Args&... args)
    {
        return m_object.query(fmt, std::forward<const Args&>(args)...);
    }
    inline Attribute& attribute() noexcept { return m_object.attribute(); }
    inline CommonCommand& commonCommand() noexcept { return m_object.commonCommand(); }

private:
    Object& m_object;
};
} // namespace OpenVisa