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
#include "Object.h"
#include "Attribute.h"
#include "Private/HiSLIP.h"
#include "Private/RawSocket.h"
#include "Private/SerialPort.h"
#include "Private/UsbTmc.h"
#include "Private/VXI11.h"

namespace OpenVisa
{
struct Object::Impl
{
    std::shared_ptr<IOBase> io;
    Attribute attr;
};

/*!
    \class      OpenVisa::Object
    \brief      仪器通讯基础库.
    \ingroup    openvisa
    \inmodule   OpenVisa
    \inheaderfile Object.h
*/

/*!
    \fn         template<IsAddress T> void OpenVisa::Object::connect(const T& addr, const std::chrono::milliseconds& openTimeout =
                std::chrono::milliseconds { 5000 }, const std::chrono::milliseconds& commandTimeout = std::chrono::milliseconds { 5000 });
    \brief      连接到地址为 \a addr 的设备，并且连接超时时间为 \a openTimeout, 通讯超时时间为 \a commandTimeout.
*/

/*!
    \fn         template <typename ...Args> inline void OpenVisa::Object::send(std::string_view fmt, const Args&... args);
    \brief      格式化发送scpi指令，参考 std::format 格式化设置, \a fmt, \a args.
    \sa         readAll, read
*/

/*!
    \fn         template <typename ...Args> inline std::string OpenVisa::Object::query(std::string_view fmt, const Args&... args);
    \brief      格式化发送查询scpi指令，参考 std::format 格式化设置, \a fmt, \a args, 并返回查询值.
    \sa         send, readAll
*/

/*!
    \brief      构造函数.
*/
Object::Object() : m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
Object::~Object() noexcept { close(); }

/*!
    \brief      关闭仪器连接.
*/
void Object::close() noexcept
{
    if (connected())
    {
        m_impl->io->close();
        m_impl->io = nullptr;
    }
}

template<>
OPENVISA_EXPORT void Object::connect<Address<AddressType::RawSocket>>(const Address<AddressType::RawSocket>& addr,
                                                                      const std::chrono::milliseconds& openTimeout /*= 5000*/,
                                                                      const std::chrono::milliseconds& commandTimeout /*= 5000*/)
{
    auto socket = std::make_shared<RawSocket>(m_impl->attr);
    socket->connect(addr, openTimeout, commandTimeout);
    m_impl->io = socket;
    afterConnected();
}

template<>
OPENVISA_EXPORT void Object::connect<Address<AddressType::SerialPort>>(const Address<AddressType::SerialPort>& addr,
                                                                       const std::chrono::milliseconds& openTimeout /*= 5000*/,
                                                                       const std::chrono::milliseconds& commandTimeout /*= 5000*/)
{
    auto serialPort = std::make_shared<SerialPort>(m_impl->attr);
    serialPort->connect(addr, openTimeout, commandTimeout);
    m_impl->io = serialPort;
    afterConnected();
}

template<>
OPENVISA_EXPORT void Object::connect<Address<AddressType::USB>>(const Address<AddressType::USB>& addr,
                                                                const std::chrono::milliseconds& openTimeout /*= 5000*/,
                                                                const std::chrono::milliseconds& commandTimeout /*= 5000*/)
{
    auto usb = std::make_shared<UsbTmc>(m_impl->attr);
    usb->connect(addr, openTimeout, commandTimeout);
    m_impl->io = usb;
    afterConnected();
}

template<>
OPENVISA_EXPORT void Object::connect<Address<AddressType::VXI11>>(const Address<AddressType::VXI11>& addr,
                                                                  const std::chrono::milliseconds& openTimeout /*= 5000*/,
                                                                  const std::chrono::milliseconds& commandTimeout /*= 5000*/)
{
    auto vxi11 = std::make_shared<VXI11>(m_impl->attr);
    vxi11->connect(addr, openTimeout, commandTimeout);
    m_impl->io = vxi11;
    afterConnected();
}

template<>
OPENVISA_EXPORT void Object::connect<Address<AddressType::HiSLIP>>(const Address<AddressType::HiSLIP>& addr,
                                                                   const std::chrono::milliseconds& openTimeout /*= 5000*/,
                                                                   const std::chrono::milliseconds& commandTimeout /*= 5000*/)
{
    auto hiSLIP = std::make_shared<HiSLIP>(m_impl->attr);
    hiSLIP->connect(addr, openTimeout, commandTimeout);
    m_impl->io = hiSLIP;
    afterConnected();
}
/*!
\brief      发送块数据 \a data 专用函数，因发送块数据时，结尾不添加终结符.
*/
void Object::sendBlockData(const std::string& data)
{
    throwNoConnection();
    m_impl->io->send(data);
}

/*!
    \brief      读取所有数据.
    \sa         read
*/
std::string Object::readAll()
{
    throwNoConnection();
    return m_impl->io->readAll();
}

/*!
    \brief      读取 \a blockSize 大小的数据，返回读到的数据，当没有更多的数据可读时，布尔值为false.
    \sa         readAll
*/
std::tuple<std::string, bool> Object::read(unsigned long blockSize)
{
    throwNoConnection();
    return { m_impl->io->read(blockSize), m_impl->io->avalible() };
}

/*!
    \brief      返回仪器是否已连接.
*/
bool Object::connected() const noexcept { return m_impl->io ? m_impl->io->connected() : false; }

/*!
    \brief      返回属性设置.
*/
Object::Attribute& Object::attribute() noexcept { return m_impl->attr; }

void Object::sendImpl(const std::string& scpi)
{
    throwNoConnection();
    m_impl->io->send(scpi + m_impl->attr.terminalChars());
}

void Object::throwNoConnection() const
{
    if (!connected())
    {
        throw std::exception("not connection");
    }
}

} // namespace OpenVisa
