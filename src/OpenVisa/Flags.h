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

#include <compare>

namespace OpenVisa
{
template<typename T>
requires std::is_enum_v<T>
class Flags
{
public:
    using Int = std::underlying_type_t<T>;
    inline Flags() {}
    inline Flags(T ev) : m_flag(static_cast<Int>(ev)) {}
    explicit inline Flags(Int value) : m_flag(value) {}
    inline Flags(const Flags& other) : m_flag(other.m_flag) {}
    inline ~Flags() {}

    inline void setFlag(T ev, bool on)
    {
        if (on)
            m_flag |= static_cast<Int>(ev);
        else
            m_flag &= ~static_cast<Int>(ev);
    }
    inline Flags operator|(T ev) const { return m_flag | static_cast<Int>(ev); }
    inline Flags& operator|=(T ev)
    {
        m_flag |= static_cast<Int>(ev);
        return *this;
    }
    inline Flags operator|(Flags evs) const { return m_flag | evs.m_flag; }
    inline Flags& operator|=(Flags evs)
    {
        m_flag |= evs.m_flag;
        return *this;
    }
    inline Flags& operator=(T ev)
    {
        m_flag = static_cast<Int>(ev);
        return *this;
    }
    inline Flags& operator=(Flags evs)
    {
        m_flag = evs.m_flag;
        return *this;
    }
    inline bool operator==(Flags evs) const { return m_flag == evs.m_flag; }
    inline bool testFlag(T ev) const { return m_flag & static_cast<Int>(ev); }
    inline Int flags() const { return m_flag; }
    inline std::strong_ordering operator<=>(const Flags& other) const
    {
        return other.m_flag == m_flag  ? std::strong_ordering::equal
               : other.m_flag < m_flag ? std::strong_ordering::less
                                       : std::strong_ordering::greater;
    }

private:
    Int m_flag;
};

template<typename T>
constexpr bool isFlag = false;
template<typename T>
constexpr bool isFlag<Flags<T>> = true;
} // namespace OpenVisa
