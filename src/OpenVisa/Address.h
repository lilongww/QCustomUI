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

enum BaudRate
{
    _1200   = 1200,
    _2400   = 2400,
    _4800   = 4800,
    _9600   = 9600,
    _19200  = 19200,
    _38400  = 38400,
    _57600  = 57600,
    _115200 = 115200
};

enum class DataBits
{
    Data5 = 5,
    Data6,
    Data7,
    Data8
};

enum class FlowControl
{
    None,
    Software,
    Hardware
};

enum class Parity
{
    None,
    Odd,
    Even
};

enum class StopBits
{
    One,
    OnePointFive,
    Two
};

template<>
struct Address<AddressType::SerialPort>
{
    [[nodiscard]] inline Address(std::string_view com,
                                 unsigned int baud       = BaudRate::_115200,
                                 DataBits dataBits       = DataBits::Data8,
                                 FlowControl flowControl = FlowControl::None,
                                 Parity parity           = Parity::None,
                                 StopBits stopBits       = StopBits::One) noexcept
        : m_com(com), m_baud(baud), m_dataBits(dataBits), m_flowControl(flowControl), m_parity(parity), m_stopBits(stopBits)
    {
    }
    [[nodiscard]] inline const std::string& portName() const noexcept { return m_com; }
    [[nodiscard]] inline unsigned int baud() const noexcept { return m_baud; }
    [[nodiscard]] inline DataBits dataBits() const noexcept { return m_dataBits; }
    [[nodiscard]] inline FlowControl flowControl() const noexcept { return m_flowControl; }
    [[nodiscard]] inline Parity parity() const noexcept { return m_parity; }
    [[nodiscard]] inline StopBits stopBits() const noexcept { return m_stopBits; }
    [[nodiscard]] auto operator<=>(const Address<AddressType::SerialPort>& other) const = default;

private:
    std::string m_com;
    unsigned int m_baud;
    DataBits m_dataBits;
    FlowControl m_flowControl;
    Parity m_parity;
    StopBits m_stopBits;
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
} // namespace OpenVisa
