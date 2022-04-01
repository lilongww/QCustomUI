﻿#pragma once

#include "Address.h"
#include "AutoLink.h"

#include <chrono>
#include <memory>

namespace OpenVisa
{
template<typename T>
concept IsAddress = IsVisaAddress<T>;

class Object
{
public:
    class CommonCommand;
    class Attribute;
    Object();
    virtual ~Object() noexcept;

    template<IsAddress T>
    void connect(const T& addr,
                 const std::chrono::milliseconds& openTimeout    = std::chrono::milliseconds { 5000 },
                 const std::chrono::milliseconds& commandTimeout = std::chrono::milliseconds { 5000 });
    void close() noexcept;
    template<typename... Args>
    inline void send(std::string_view fmt, const Args&... args);
    void sendBlockData(const std::string& data);
    std::string readAll();
    std::tuple<std::string, bool> read(unsigned long blockSize);
    template<typename... Args>
    inline std::string query(std::string_view fmt, const Args&... args);
    bool connected() const noexcept;
    Attribute& attribute() noexcept;

protected:
    virtual void afterConnected() {};

private:
    void sendImpl(const std::string& scpi);
    void throwNoConnection() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

template<typename... Args>
inline void Object::send(std::string_view fmt, const Args&... args)
{
    sendImpl(std::format(fmt, std::forward<const Args&>(args)...));
}

template<typename... Args>
std::string Object::query(std::string_view fmt, const Args&... args)
{
    send(fmt, std::forward<const Args&>(args)...);
    return readAll();
}

template<typename T>
struct VisaAdl
{
    inline static std::string toScpi(const T& arg)
    {
        static_assert(std::_Always_false<T>, "Please define your ADL function: std::string toScpi(YourType).");
    }
    inline static void fromScpi(const std::string& ret, T& out)
    {
        static_assert(std::_Always_false<T>, "Please define your ADL function: void fromScpi(const std::string&, YourType&).");
    }
};
} // namespace OpenVisa

#define VISA_ENUM_ADL_DECLARE(EnumBegin, EnumEnd, ...)                                                                                     \
    template<>                                                                                                                             \
    struct VisaAdl<decltype(EnumBegin)>                                                                                                    \
    {                                                                                                                                      \
        using Enum = decltype(EnumBegin);                                                                                                  \
        using Int  = typename std::underlying_type<Enum>::type;                                                                            \
        inline static constexpr std::initializer_list<std::string_view> m_enumStrings {##__VA_ARGS__ };                                    \
        static_assert(std::is_same_v<Enum, decltype(EnumEnd)>, "EnumBegin is not same type of EnumEnd");                                   \
        static_assert(m_enumStrings.size() - 1 == static_cast<Int>(EnumEnd) - static_cast<Int>(EnumBegin),                                 \
                      "Not continus enum or strings not enough.");                                                                         \
        inline static std::string toScpi(Enum e)                                                                                           \
        {                                                                                                                                  \
            const auto& view = (*(m_enumStrings.begin() + (static_cast<Int>(e) - static_cast<Int>(EnumBegin))));                           \
            return std::string(view.data(), view.size());                                                                                  \
        }                                                                                                                                  \
        inline static void fromScpi(const std::string& scpi, Enum& e)                                                                      \
        {                                                                                                                                  \
            if (auto it = std::ranges::find_if(m_enumStrings, [&](const auto& str) { return scpi.starts_with(str); });                     \
                it != m_enumStrings.end())                                                                                                 \
                e = static_cast<Enum>(std::distance(m_enumStrings.begin(), it) + static_cast<Int>(EnumBegin));                             \
            else                                                                                                                           \
                throw std::exception("Unknown enum.");                                                                                     \
        }                                                                                                                                  \
    }