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

namespace OpenVisa
{
template<typename T>
class BinaryArray
{
public:
    using Type     = T;
    using Iterator = const Type*;
    [[nodiscard]] inline BinaryArray(std::string&& buffer) : m_temp(buffer)
    {
        if (m_temp.empty())
            return;
        if (!m_temp.starts_with('#'))
            return;
        // ascii - binary
        auto byteOfByteCount      = m_temp[1] - 48;
        auto byteCountBeginOffset = 2;
        auto dataBeginOffset      = byteCountBeginOffset + byteOfByteCount;
        std::size_t byteCount;
        std::from_chars(m_temp.c_str() + byteCountBeginOffset, m_temp.c_str() + dataBeginOffset, byteCount);
        m_size = byteCount / sizeof(Type);
        m_data = reinterpret_cast<const Type*>(m_temp.c_str() + dataBeginOffset);
    }
    [[nodiscard]] inline BinaryArray(BinaryArray&& other)
    {
        m_temp = std::move(other.m_temp);
        m_data = other.m_data;
        m_size = other.m_size;
    }
    [[nodiscard]] inline std::size_t size() const { return m_size; }
    [[nodiscard]] inline const Type* data() const { return m_data; }
    [[nodiscard]] inline Iterator begin() const { return m_data; }
    [[nodiscard]] inline Iterator end() const { return m_data + m_size; }
    [[nodiscard]] inline bool empty() const { return !m_size; }
    [[nodiscard]] inline operator std::vector<Type>() const
    {
        std::vector<Type> vec(m_size);
        memcpy(vec.data(), m_data, m_size * sizeof(Type));
        return vec;
    }

private:
    std::string m_temp;
    const Type* m_data { nullptr };
    std::size_t m_size { 0 };
};
} // namespace OpenVisa
