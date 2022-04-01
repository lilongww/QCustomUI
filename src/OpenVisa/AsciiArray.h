#pragma once

#include <algorithm>
#include <ranges>

namespace OpenVisa
{
template <typename T>
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
