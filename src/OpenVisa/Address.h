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

#include <format>
#include <string>
#include <string_view>
#include <variant>

namespace OpenVisa
{
enum class AddressType
{
    RawSocket,
    SerialPort,
    USB,
    VXI11,
    HiSLIP
};

template<AddressType type>
struct Address
{
    static_assert(sizeof(type), "Unknown visa address");
};

template<>
struct Address<AddressType::RawSocket>
{
    [[nodiscard]] inline Address(std::string_view ip, unsigned short port) noexcept : m_ip(ip), m_port(port) {}
    [[nodiscard]] inline const std::string& ip() const noexcept { return m_ip; }
    [[nodiscard]] inline unsigned short port() const noexcept { return m_port; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::RawSocket>& other) const = default;

private:
    std::string m_ip;
    unsigned short m_port;
};

template<>
struct Address<AddressType::SerialPort>
{
    [[nodiscard]] inline Address(std::string_view com) noexcept : m_com(com) {}
    [[nodiscard]] inline const std::string& portName() const noexcept { return m_com; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::SerialPort>& other) const = default;

private:
    std::string m_com;
};

template<>
struct Address<AddressType::USB>
{
    [[nodiscard]] inline Address(unsigned short vid, unsigned short pid, std::string_view sn) noexcept
        : m_vid(vid), m_pid(pid), m_serialNumber(sn)
    {
    }
    [[nodiscard]] inline unsigned short vendorId() const noexcept { return m_vid; }
    [[nodiscard]] inline unsigned short productId() const noexcept { return m_pid; }
    [[nodiscard]] inline std::string serialNumber() const noexcept { return m_serialNumber; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::USB>& other) const = default;

private:
    unsigned short m_vid;
    unsigned short m_pid;
    std::string m_serialNumber;
};

template<>
struct Address<AddressType::VXI11>
{
    [[nodiscard]] inline Address(std::string_view ip, std::string_view subAddress = "inst0") noexcept : m_ip(ip), m_subAddress(subAddress)
    {
    }
    [[nodiscard]] inline const std::string& ip() const noexcept { return m_ip; }
    [[nodiscard]] inline const std::string& subAddress() const noexcept { return m_subAddress; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::VXI11>& other) const = default;

private:
    std::string m_ip;
    std::string m_subAddress;
};

template<>
struct Address<AddressType::HiSLIP>
{
    [[nodiscard]] inline Address(std::string_view ip, std::string_view subAddress = "hislip0", unsigned short port = 4880) noexcept
        : m_ip(ip), m_subAddress(subAddress), m_port(port)
    {
    }
    [[nodiscard]] inline const std::string& ip() const noexcept { return m_ip; }
    [[nodiscard]] inline unsigned short port() const noexcept { return m_port; }
    [[nodiscard]] inline const std::string& subAddress() const noexcept { return m_subAddress; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::HiSLIP>& other) const = default;

private:
    std::string m_ip;
    unsigned short m_port;
    std::string m_subAddress;
};

template<typename T>
constexpr bool IsVisaAddress = std::false_type {};
template<AddressType T>
constexpr bool IsVisaAddress<Address<T>> = std::true_type {};

using AddressVariant = std::variant<std::monostate,
                                    Address<AddressType::RawSocket>,
                                    Address<AddressType::SerialPort>,
                                    Address<AddressType::USB>,
                                    Address<AddressType::VXI11>,
                                    Address<AddressType::HiSLIP>>;

template<typename T>
requires IsAddress<T> || std::same_as<std::string, T> || std::is_array_v<T> || std::same_as<const char*, T> || std::same_as<char*, T>
struct AddressHelper
{
    AddressHelper(const T& addr) : address(addr) {}
    const T& address;
};

template<typename T, size_t N>
struct AddressHelper<T[N]>
{
    AddressHelper(const T (&addr)[N]) : address(addr) {}
    std::string address;
};

template<typename T>
struct AddressHelper<const T*>
{
    AddressHelper(const T* addr) : address(addr) {}
    std::string address;
};

template<typename T>
AddressHelper(T*) -> AddressHelper<const T*>;

} // namespace OpenVisa
