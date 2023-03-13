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
#include "Attribute.h"
#include "Private/SerialPort.h"

namespace OpenVisa
{
struct Object::Attribute::Impl
{
    std::shared_ptr<IOBase>* io { nullptr };
    std::string termChars { "\n" };
    std::chrono::milliseconds timeout { 2000 };
    bool autoAppendTerminalChars { true };
    std::optional<unsigned int> baud;
    std::optional<DataBits> dataBits;
    std::optional<FlowControl> flowControl;
    std::optional<Parity> parity;
    std::optional<StopBits> stopBits;
    std::string deviceName;
    inline Impl(std::shared_ptr<IOBase>* _io) : io(_io) {}
    inline std::shared_ptr<SerialPort> asSerialPort() const { return io ? std::dynamic_pointer_cast<SerialPort>(*io) : nullptr; }
};

/*!
    \class      OpenVisa::Object::Attribute
    \brief      驱动属性集.
    \ingroup    openvisa
    \inmodule   OpenVisa
    \inheaderfile Attribute.h
*/

/*!
    \enum       OpenVisa::BaudRate
                串口常用波特率定义.
    \value      _1200
    \value      _2400
    \value      _4800
    \value      _9600
    \value      _19200
    \value      _38400
    \value      _57600
    \value      _115200
*/

/*!
    \enum       OpenVisa::DataBits
                数据位定义.
    \value      Data5
    \value      Data6
    \value      Data7
    \value      Data8
*/

/*!
    \enum       OpenVisa::FlowControl
                流控制定义.
    \value      None        无.
    \value      Software    软件.
    \value      Hardware    硬件.
*/

/*!
    \enum       OpenVisa::Parity
                校验设置.
    \value      None        无.
    \value      Odd         奇校验.
    \value      Even        偶校验.
*/

/*!
    \enum       OpenVisa::StopBits
                停止位设置.
    \value      One             1.
    \value      OnePointFive    1.5.
    \value      Two             2.
*/

/*!
    \brief      构造函数 \a io.
*/
Object::Attribute::Attribute(std::shared_ptr<IOBase>* io) : m_impl(std::make_unique<Impl>(io)) {}

/*!
    \brief      析构函数.
*/
Object::Attribute::~Attribute() {}

/*!
    \brief      设置终结符 \a chars, 默认为 '\n'(0x0A).
    \sa         terminalChars
*/
void Object::Attribute::setTerminalChars(std::string_view chars) { m_impl->termChars = chars; }

/*!
    \brief      返回终结符.
    \sa         setTerminalChars
*/
const std::string& Object::Attribute::terminalChars() const { return m_impl->termChars; }

/*!
    \brief      设置通信超时时间 \a timeout.
    \sa         timeout
*/
void Object::Attribute::setTimeout(const std::chrono::milliseconds& timeout) { m_impl->timeout = timeout; }

/*!
    \brief      返回通信超时时间.
    \sa         setTimeout
*/
const std::chrono::milliseconds& Object::Attribute::timeout() const { return m_impl->timeout; }

/*!
    \brief      设置是否启用发送数据时是否自动添加终结符 \a en, 默认为true.
    \sa         autoAppendTerminalChars
*/
void Object::Attribute::setAutoAppendTerminalChars(bool en) { m_impl->autoAppendTerminalChars = en; }

/*!
    \brief      返回是否启用发送数据时是否自动添加终结符.
    \sa         setAutoAppendTerminalChars
*/
bool Object::Attribute::autoAppendTerminalChars() const { return m_impl->autoAppendTerminalChars; }

/*!
    \brief      设置波特率 \a baud, 仅串口连接时有效.
    \sa         baudRate
*/
void Object::Attribute::setBaudRate(unsigned int baud)
{
    m_impl->baud = baud;
    if (auto sp = m_impl->asSerialPort(); sp)
        sp->setBaudRate(baud);
}

/*!
    \brief      返回波特率, 仅串口连接时有效，如果没有设置则返回 std::nullopt.
    \sa
*/
std::optional<unsigned int> Object::Attribute::baudRate() const { return m_impl->baud; }

/*!
    \brief      设置数据位 \a bits, 仅串口连接时有效.
    \sa         dataBits
*/
void Object::Attribute::setDataBits(DataBits bits)
{
    m_impl->dataBits = bits;
    if (auto sp = m_impl->asSerialPort(); sp)
        sp->setDataBits(bits);
}

/*!
    \brief      返回数据位, 仅串口连接时有效，如果没有设置则返回 std::nullopt.
    \sa         setDataBits
*/
std::optional<DataBits> Object::Attribute::dataBits() const { return m_impl->dataBits; }

/*!
    \brief      设置流控制 \a fc, 仅串口连接时有效.
    \sa         flowControl
*/
void Object::Attribute::setFlowControl(FlowControl fc)
{
    m_impl->flowControl = fc;
    if (auto sp = m_impl->asSerialPort(); sp)
        sp->setFlowControl(fc);
}

/*!
    \brief      返回流控制, 仅串口连接时有效，如果没有设置则返回 std::nullopt.
    \sa         setFlowControl
*/
std::optional<FlowControl> Object::Attribute::flowControl() const { return m_impl->flowControl; }

/*!
    \brief      设置校验规则 \a p, 仅串口连接时有效.
    \sa         parity
*/
void Object::Attribute::setParity(Parity p)
{
    m_impl->parity = p;
    if (auto sp = m_impl->asSerialPort(); sp)
        sp->setParity(p);
}

/*!
    \brief      返回校验规则, 仅串口连接时有效，如果没有设置则返回 std::nullopt.
    \sa         setParity
*/
std::optional<Parity> Object::Attribute::parity() const { return m_impl->parity; }

/*!
    \brief      返回停止位, 仅串口连接时有效，如果没有设置则返回 std::nullopt.
    \sa         setStopBits
*/
std::optional<StopBits> Object::Attribute::stopBits() const { return m_impl->stopBits; }

/*!
    \brief      设置停止位 \a bits, 仅串口连接时有效.
    \sa         stopBits
*/
void Object::Attribute::setStopBits(StopBits bits)
{
    m_impl->stopBits = bits;
    if (auto sp = m_impl->asSerialPort(); sp)
        sp->setStopBits(bits);
}

/*!
    \brief      设置设备名称 \a name, 该设置不是必须的，如果设置了设备名称，抛出异常时，
                异常描述将以该设备名称为前缀，用于多台设备控制时的区分，若未设置设备名，则抛出的异常描述无前缀.
    \sa         deviceName
*/
void Object::Attribute::setDeviceName(std::string_view name) { m_impl->deviceName = name; }

/*!
    \brief      返回设备名称.
    \sa         setDeviceName
*/
const std::string& Object::Attribute::deviceName() const { return m_impl->deviceName; }

} // namespace OpenVisa
