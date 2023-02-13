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

#include "Address.h"
#include "openvisa_global.h"

#include <chrono>
#include <format>
#include <memory>
#include <string>
#include <string_view>

namespace OpenVisa
{
template<typename T>
concept IsAddress = IsVisaAddress<T>;

class OPENVISA_EXPORT Object
{
public:
    class CommonCommand;
    class Attribute;
    Object();
    virtual ~Object() noexcept;

    template<typename T>
    requires IsAddress<T> || std::same_as<std::string, T> || std::is_array_v<T> || std::same_as<const char*, T> || std::same_as<char*, T>
    inline void connect(const T& addr,
                        const std::chrono::milliseconds& openTimeout    = std::chrono::milliseconds { 5000 },
                        const std::chrono::milliseconds& commandTimeout = std::chrono::milliseconds { 5000 })
    {
        connectImpl(AddressHelper(addr).address, openTimeout, commandTimeout);
    }
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
    CommonCommand& commonCommand() noexcept;
    static std::vector<std::string> listSerialPorts();
    static std::vector<Address<AddressType::USB>> listUSB();
    template<IsAddress T>
    static std::string toVisaAddressString(const T& addr);
    static AddressVariant fromVisaAddressString(const std::string& str);

protected:
    virtual void afterConnected() {};
    template<typename T>
    requires IsAddress<T> || std::same_as<std::string, T>
    void connectImpl(const T& addr, const std::chrono::milliseconds& openTimeout, const std::chrono::milliseconds& commandTimeout);

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
    sendImpl(std::vformat(fmt, std::make_format_args(std::forward<const Args&>(args)...)));
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
