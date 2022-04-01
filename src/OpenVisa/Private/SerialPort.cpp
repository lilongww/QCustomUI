#include "SerialPort.h"

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
    std::chrono::milliseconds timeout;
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

void SerialPort::connect(const Address<AddressType::SerialPort>& addr,
                         const std::chrono::milliseconds& openTimeout,
                         const std::chrono::milliseconds& commandTimeout)
{
    m_impl->timeout = commandTimeout;
    m_impl->serialPort.open(addr.portName());
    m_impl->serialPort.set_option(boost::asio::serial_port::baud_rate { addr.baud() });
    m_impl->serialPort.set_option(boost::asio::serial_port::character_size { static_cast<unsigned int>(addr.dataBits()) });
    m_impl->serialPort.set_option(
        boost::asio::serial_port::flow_control { static_cast<boost::asio::serial_port::flow_control::type>(addr.flowControl()) });
    m_impl->serialPort.set_option(boost::asio::serial_port::parity { static_cast<boost::asio::serial_port::parity::type>(addr.parity()) });
    m_impl->serialPort.set_option(
        boost::asio::serial_port::stop_bits { static_cast<boost::asio::serial_port::stop_bits::type>(addr.stopBits()) });
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

    if (!mutex->try_lock_for(m_impl->timeout))
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

    if (!mutex->try_lock_for(m_impl->timeout))
    {
        m_impl->serialPort.cancel();
        throw std::exception("read timeout");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->serialPort.close();
        boost::asio::detail::throw_error(*error, "read");
    }
    std::string buffer(boost::asio::buffer_cast<const char*>(m_impl->readBuffer.data()), m_impl->readBuffer.size());
    m_impl->readBuffer.consume(buffer.size());
    return buffer;
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

    if (!mutex->try_lock_for(m_impl->timeout))
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
} // namespace OpenVisa
