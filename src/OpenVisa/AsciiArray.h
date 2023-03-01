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

#include <algorithm>
#include <charconv>
#include <cmath>
#include <complex>
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
    static_assert(std::is_arithmetic_v<T>);
    using Type = T;
    [[nodiscard]] inline AsciiArray(std::string&& buffer, std::size_t offset = 0) : m_temp(buffer), m_offset(offset) {}
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

template<typename Container>
class _ComplexBackInserter
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;
    using container_type    = Container;
    using difference_type   = ptrdiff_t;

    constexpr explicit _ComplexBackInserter(Container& cont) noexcept : m_container(std::addressof(cont)) {}
    constexpr _ComplexBackInserter& operator=(const typename Container::value_type::value_type& val)
    {
        if (m_real)
            m_container->push_back(typename Container::value_type { val, 0.0 });
        else
            m_container->back().imag(val);
        m_real = !m_real;
        return *this;
    }
    constexpr _ComplexBackInserter& operator=(typename Container::value_type::value_type&& val)
    {
        if (m_real)
            m_container->push_back(typename Container::value_type { std::move(val), 0.0 });
        else
            m_container->back().imag(val);
        m_real = !m_real;
        return *this;
    }
    [[nodiscard]] constexpr _ComplexBackInserter& operator*() noexcept { return *this; }
    constexpr _ComplexBackInserter& operator++() noexcept { return *this; }
    constexpr _ComplexBackInserter operator++(int) noexcept { return *this; }

protected:
    Container* m_container;
    bool m_real { true };
};

template<typename Container>
constexpr _ComplexBackInserter<Container> _complexBackInsert(Container& con)
{
    return _ComplexBackInserter<Container>(con);
}

template<typename T>
class AsciiArray<std::complex<T>>
{
public:
    static_assert(std::is_arithmetic_v<T>);
    using Type = std::complex<T>;
    [[nodiscard]] inline AsciiArray(std::string&& buffer, std::size_t offset = 0) : m_temp(buffer), m_offset(offset) {}
    [[nodiscard]] inline operator std::vector<Type>() const&&
    {
        std::vector<Type> vec;
        auto li   = std::counted_iterator(m_temp.begin() + m_offset, m_temp.size() - m_offset);
        auto view = std::ranges::subrange(li, std::default_sentinel) | std::views::split(',') |
                    std::views::transform(
                        [](auto&& rng) -> std::optional<T>
                        {
                            auto str = std::string_view(&*rng.begin(), std::ranges::distance(rng));
                            T value;
                            auto ret = std::from_chars(str.starts_with('+') ? str.data() + 1 : str.data(), str.data() + str.size(), value);
                            if (std::make_error_code(ret.ec))
                                return std::nullopt;
                            return value;
                        }) |
                    std::views::filter([](auto v) { return v.has_value(); });
        std::ranges::transform(view, _complexBackInsert(vec), [](auto value) { return *value; });
        return vec;
    }

private:
    std::size_t m_offset { 0 };
    std::string m_temp;
};
} // namespace OpenVisa
