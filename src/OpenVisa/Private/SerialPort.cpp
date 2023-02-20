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
#include "SerialPort.h"
#include "OpenVisaConfig.h"

#include <boost/asio.hpp>
#ifdef WIN32
#include <windows.h>
#elif defined(__linux__)
#include <linux/serial.h>
#endif

namespace OpenVisa
{
struct SerialPort::Impl
{
    boost::asio::io_context io;
    boost::asio::serial_port serialPort { io };
    boost::asio::io_context::work worker { io };
    std::jthread thread;
    std::string writeBuffer;
    boost::asio::streambuf readBuffer;
};

SerialPort::SerialPort(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>())
{
    m_impl->thread = std::jthread(
        static_cast<boost::asio::io_context::count_type (boost::asio::io_context::*)()>(&boost::asio::io_context::run), &m_impl->io);
}

SerialPort::~SerialPort() noexcept
{
    if (m_impl->thread.joinable())
    {
        if (m_impl->serialPort.is_open())
            m_impl->serialPort.close();
        m_impl->io.stop();
    }
}

void SerialPort::connect(const Address<AddressType::SerialPort>& addr, const std::chrono::milliseconds& openTimeout)
{
    auto asrlId      = OpenVisaConfig::instance().toAsrl(addr.portName());
    const auto& asrl = asrlId ? OpenVisaConfig::instance().fromAsrl(*asrlId) : std::nullopt;

    m_impl->serialPort.open(addr.portName());
    setBaudRate(m_attr.baudRate() ? *m_attr.baudRate() : asrl ? asrl->baud : 9600);
    setDataBits(m_attr.dataBits() ? *m_attr.dataBits() : asrl ? asrl->dataBits : DataBits::Data8);
    setFlowControl(m_attr.flowControl() ? *m_attr.flowControl() : asrl ? asrl->flowControl : FlowControl::None);
    setParity(m_attr.parity() ? *m_attr.parity() : asrl ? asrl->parity : Parity::None);
    setStopBits(m_attr.stopBits() ? *m_attr.stopBits() : asrl ? asrl->stopBits : StopBits::One);
}

void SerialPort::send(const std::string& buffer) const
{
    m_impl->writeBuffer = buffer;
    auto mutex          = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error = std::make_shared<boost::system::error_code>();
    m_impl->io.post(
        [=]()
        {
            m_impl->serialPort.async_write_some(boost::asio::buffer(m_impl->writeBuffer),
                                                [=](const boost::system::error_code& e, std::size_t)
                                                {
                                                    *error = e;
                                                    mutex->unlock();
                                                });
        });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->serialPort.cancel();
        throw std::exception("send timeout");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->serialPort.close();
        boost::asio::detail::throw_error(*error, "send");
    }
}

std::string SerialPort::readAll() const
{
    std::shared_ptr<void> scope(nullptr, [=](void*) { m_impl->readBuffer.consume(m_impl->readBuffer.size()); });
    auto header = read(2);
    {
        std::ostream out(&m_impl->readBuffer);
        out.write(header.c_str(), header.size());
    }
    if (*header.begin() == '#' && header.size() == 2 && std::isdigit(header[1]))
    {
        return readAllBlockData(header[1] - '0');
    }
    else
    {
        return readAllAscii();
    }
}

std::string SerialPort::read(size_t size) const
{
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error  = std::make_shared<boost::system::error_code>();
    auto buffer = std::make_shared<std::string>();
    buffer->resize(size);
    m_impl->io.post(
        [=]()
        {
            m_impl->serialPort.async_read_some(boost::asio::buffer(*buffer),
                                               [=](const boost::system::error_code& e, std::size_t s)
                                               {
                                                   buffer->resize(s);
                                                   *error = e;
                                                   mutex->unlock();
                                               });
        });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->serialPort.cancel();
        throw std::exception("read timeout");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->serialPort.close();
        boost::asio::detail::throw_error(*error, "read");
    }
    return *buffer;
}

void SerialPort::close() noexcept
{
    boost::system::error_code ec;
    m_impl->serialPort.close(ec);
}

bool SerialPort::connected() const noexcept { return m_impl->serialPort.is_open(); }

size_t SerialPort::avalible() const noexcept
{
    if (!m_impl->serialPort.is_open())
        return 0;
    auto fd = m_impl->serialPort.native_handle();
#ifdef WIN32
    COMSTAT cs;
    if (!ClearCommError(fd, NULL, &cs))
    {
        // std::stringstream ss;
        // ss << "Error while checking status of the serial port: " << GetLastError();
        return 0;
    }
    return static_cast<size_t>(cs.cbInQue);
#elif defined(__linux__)
    int count = 0;
    if (-1 == ioctl(fd, TIOCINQ, &count))
    {
        // THROW(IOException, errno);
        return 0;
    }
    return static_cast<size_t>(count);
#else
#error avalible not defined.
#endif
}

void SerialPort::setBaudRate(unsigned int baud) { m_impl->serialPort.set_option(boost::asio::serial_port::baud_rate { baud }); }

void SerialPort::setDataBits(DataBits bits)
{
    m_impl->serialPort.set_option(boost::asio::serial_port::character_size { static_cast<unsigned int>(bits) });
}

void SerialPort::setFlowControl(FlowControl fc)
{
    m_impl->serialPort.set_option(boost::asio::serial_port::flow_control { static_cast<boost::asio::serial_port::flow_control::type>(fc) });
}

void SerialPort::setParity(Parity p)
{
    m_impl->serialPort.set_option(boost::asio::serial_port::parity { static_cast<boost::asio::serial_port::parity::type>(p) });
}

void SerialPort::setStopBits(StopBits bits)
{
    m_impl->serialPort.set_option(boost::asio::serial_port::stop_bits { static_cast<boost::asio::serial_port::stop_bits::type>(bits) });
}

std::string SerialPort::readAllAscii() const
{
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error = std::make_shared<boost::system::error_code>();
    auto size  = std::make_shared<std::size_t>(0);
    m_impl->io.post(
        [=]()
        {
            boost::asio::async_read_until(m_impl->serialPort,
                                          m_impl->readBuffer,
                                          m_attr.terminalChars(),
                                          [=](const boost::system::error_code& e, std::size_t s)
                                          {
                                              *size  = s;
                                              *error = e;
                                              mutex->unlock();
                                          });
        });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->serialPort.cancel();
        throw std::exception("read timeout");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->serialPort.close();
        boost::asio::detail::throw_error(*error, "readAllAscii");
    }
    std::string buffer(boost::asio::buffer_cast<const char*>(m_impl->readBuffer.data()), m_impl->readBuffer.size());
    return buffer;
}

std::string SerialPort::readAllBlockData(unsigned char bufferStringLen) const
{
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error = std::make_shared<boost::system::error_code>();
    auto size  = std::make_shared<std::size_t>(0);
    m_impl->io.post(
        [=]()
        {
            std::scoped_lock lock(std::adopt_lock, *mutex);
            std::string buffer(static_cast<size_t>(bufferStringLen), '0');
            boost::asio::read(m_impl->serialPort, boost::asio::buffer(buffer), boost::asio::transfer_exactly(bufferStringLen), *error);
            if (*error)
            {
                return;
            }
            else
            {
                std::ostream os(&m_impl->readBuffer);
                os.write(buffer.c_str(), buffer.size());
            }
            size_t len;
            try
            {
                len = std::stoull(buffer);
            }
            catch (const std::exception&)
            {
                // str不是一个数字，即非visa二进制传输格式.
                while (avalible())
                {
                    boost::asio::read(m_impl->serialPort, m_impl->readBuffer, *error);
                    if (*error)
                    {
                        return;
                    }
                }
            }
            boost::asio::read(m_impl->serialPort, m_impl->readBuffer, boost::asio::transfer_at_least(len), *error);
        });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->serialPort.cancel();
        throw std::exception("read timeout");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->serialPort.close();
        boost::asio::detail::throw_error(*error, "readAllBlockData");
    }
    std::string buffer(boost::asio::buffer_cast<const char*>(m_impl->readBuffer.data()), m_impl->readBuffer.size());
    return buffer;
}
} // namespace OpenVisa
