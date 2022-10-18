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
#include <cmath>
#include <optional>
#include <ranges>
#include <system_error>
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
                    std::views::transform(
                        [](auto&& rng) -> std::optional<T>
                        {
                            auto str = std::string_view(&*rng.begin(), std::ranges::distance(rng));
                            Type value;
                            auto ret = std::from_chars(str.starts_with('+') ? str.data() + 1 : str.data(), str.data() + str.size(), value);
                            if (std::make_error_code(ret.ec))
                                return std::nullopt;
                            return value;
                        }) |
                    std::views::filter([](auto v) { return v.has_value(); });
        std::ranges::transform(view, std::back_inserter(vec), [](auto value) { return *value; });
        return vec;
    }

private:
    std::size_t m_offset { 0 };
    std::string m_temp;
};
} // namespace OpenVisa
