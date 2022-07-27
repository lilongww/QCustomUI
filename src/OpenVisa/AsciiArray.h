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

#include <algorithm>
#include <charconv>
#include <ranges>
#include <vector>

namespace OpenVisa
{
template<typename T>
class AsciiArray
{
public:
    using Type = T;
    inline AsciiArray(std::string&& buffer, std::size_t offset = 0) : m_temp(buffer), m_offset(offset) {}
    [[nodiscard]] inline operator std::vector<T>() const&&
    {
        std::vector<T> vec;
        auto li   = std::counted_iterator(m_temp.begin() + m_offset, m_temp.size() - m_offset);
        auto view = std::ranges::subrange(li, std::default_sentinel) | std::views::split(',') |
                    std::views::transform([](auto&& rng) { return std::string_view(&*rng.begin(), std::ranges::distance(rng)); });

        std::ranges::transform(view,
                               std::back_inserter(vec),
                               [](const auto& str)
                               {
                                   Type value;
                                   std::from_chars(str.data(), str.data() + str.size(), value);
                                   return value;
                               });
        return vec;
    }

private:
    std::size_t m_offset { 0 };
    std::string m_temp;
};
} // namespace OpenVisa
