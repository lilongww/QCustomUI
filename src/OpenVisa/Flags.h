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
    inline Flags(T ev) : m_flag(static_cast<Int>(ev)) {}
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

protected:
    inline Flags(Int flag) : m_flag(flag) {}

private:
    Int m_flag;
};
} // namespace OpenVisa
