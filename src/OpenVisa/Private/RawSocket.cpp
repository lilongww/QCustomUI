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
#include "RawSocket.h"

#include <boost/asio.hpp>

#include <mutex>
#include <thread>

namespace OpenVisa
{
struct RawSocket::Impl
{
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket socket { io };
    boost::asio::io_context::work worker { io };
    std::jthread thread;
    std::string writeBuffer;
    boost::asio::streambuf readBuffer;
};

RawSocket::RawSocket(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>())
{
    m_impl->thread = std::jthread(
        static_cast<boost::asio::io_context::count_type (boost::asio::io_context::*)()>(&boost::asio::io_context::run), &m_impl->io);
}

RawSocket::~RawSocket() noexcept
{
    if (m_impl->thread.joinable())
    {
        if (m_impl->socket.is_open())
            m_impl->socket.close();
        m_impl->io.stop();
    }
}

void RawSocket::connect(const Address<AddressType::RawSocket>& addr, const std::chrono::milliseconds& openTimeout)
{
    auto error = std::make_shared<boost::system::error_code>();
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    m_impl->socket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(addr.ip()), addr.port()),
                                 [=](const boost::system::error_code& e)
                                 {
                                     *error = e;
                                     mutex->unlock();
                                 });
    if (!mutex->try_lock_for(openTimeout))
    {
        m_impl->socket.close();
        throw std::exception("Connect timeout.");
    }
    if (*error != boost::system::errc::success)
    {
        boost::asio::detail::throw_error(*error, "connect");
    }
}

void RawSocket::send(const std::string& buffer) const
{
    m_impl->writeBuffer = buffer;
    auto mutex          = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error = std::make_shared<boost::system::error_code>();
    m_impl->socket.async_write_some(boost::asio::buffer(m_impl->writeBuffer),
                                    [=](const boost::system::error_code& e, std::size_t)
                                    {
                                        *error = e;
                                        mutex->unlock();
                                    });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->socket.cancel();
        throw std::exception("Send timeout.");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->socket.close();
        boost::asio::detail::throw_error(*error, "send");
    }
}

std::string RawSocket::readAll() const
{
    std::shared_ptr<void> scope(nullptr, [=](void*) { m_impl->readBuffer.consume(m_impl->readBuffer.size()); });
    if (m_attr.terminalCharsEnable())
    {
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
    else
    {
        std::string buffer;
        do
        {
            buffer.append(read(1024));
        } while (avalible());
        return buffer;
    }
}

std::string RawSocket::read(size_t size) const
{
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error  = std::make_shared<boost::system::error_code>();
    auto buffer = std::make_shared<std::string>();
    buffer->resize(size);
    m_impl->socket.async_read_some(boost::asio::buffer(*buffer),
                                   [=](const boost::system::error_code& e, std::size_t s)
                                   {
                                       buffer->resize(s);
                                       *error = e;
                                       mutex->unlock();
                                   });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->socket.cancel();
        throw std::exception("Read timeout.");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->socket.close();
        boost::asio::detail::throw_error(*error, "read");
    }
    return *buffer;
}

void RawSocket::close() noexcept
{
    boost::system::error_code ec;
    m_impl->socket.close(ec);
}

bool RawSocket::connected() const noexcept { return m_impl->socket.is_open(); }

size_t RawSocket::avalible() const noexcept
{
    boost::system::error_code e;
    return m_impl->socket.is_open() ? m_impl->socket.available(e) : 0UL;
}

std::string RawSocket::readAllAscii() const
{
    auto mutex = std::make_shared<std::timed_mutex>();
    mutex->lock();
    auto error = std::make_shared<boost::system::error_code>();
    auto size  = std::make_shared<std::size_t>(0);
    boost::asio::async_read_until(m_impl->socket,
                                  m_impl->readBuffer,
                                  m_attr.terminalChars(),
                                  [=](const boost::system::error_code& e, std::size_t s)
                                  {
                                      *size  = s;
                                      *error = e;
                                      mutex->unlock();
                                  });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->socket.cancel();
        throw std::exception("Read timeout.");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->socket.close();
        boost::asio::detail::throw_error(*error, "readAllAscii");
    }
    std::string buffer(boost::asio::buffer_cast<const char*>(m_impl->readBuffer.data()), m_impl->readBuffer.size());
    return buffer;
}

std::string RawSocket::readAllBlockData(unsigned char bufferStringLen) const
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
            boost::asio::read(m_impl->socket, boost::asio::buffer(buffer), boost::asio::transfer_exactly(bufferStringLen), *error);
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
                while (m_impl->socket.available())
                {
                    boost::asio::read(m_impl->socket, m_impl->readBuffer, *error);
                    if (*error)
                    {
                        return;
                    }
                }
            }
            boost::asio::read(m_impl->socket, m_impl->readBuffer, boost::asio::transfer_at_least(len), *error);
        });

    if (!mutex->try_lock_for(m_attr.timeout()))
    {
        m_impl->socket.cancel();
        throw std::exception("Read timeout.");
    }
    if (*error != boost::system::errc::success)
    {
        m_impl->socket.close();
        boost::asio::detail::throw_error(*error, "readAllBlockData");
    }
    std::string buffer(boost::asio::buffer_cast<const char*>(m_impl->readBuffer.data()), m_impl->readBuffer.size());
    return buffer;
}
} // namespace OpenVisa
