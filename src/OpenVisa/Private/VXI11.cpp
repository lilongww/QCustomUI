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
#include "VXI11.h"
#include "RawSocket.h"
#include "VXI11Protocol.h"

#include <limits>
#include <random>

namespace OpenVisa
{
struct VXI11::Impl
{
    RawSocket socket;
    std::chrono::milliseconds ioTimeout;

    unsigned short corePort { 0 };
    unsigned short abortPort { 0 };
    unsigned short interruptPort { 0 };

    std::random_device r;
    std::mt19937 e1 { r() };
    std::uniform_int_distribution<> uniform_dist;
    unsigned int xid;
    unsigned int clientId { generateXid() };
    bool connected { false };
    VXI::DeviceLink linkId;
    size_t maxBufferSize;
    bool avalibe { false };
    inline Impl(Object::Attribute const& attr) : socket(attr) {}
    inline unsigned int generateXid() { return xid = uniform_dist(e1); }
    inline RPCBuffer read()
    {
        RPCBuffer buffer;
        do
        {
            buffer.std::string::append(socket.read(1024));
        } while (socket.avalible());
        return buffer;
    }
    VXI::Resp::CreateLink createLink(const VXI::Req::CreateLink& link);
    VXI::Resp::DestroyLink destroyLink(const VXI::Req::DestroyLink& link);
    VXI::Resp::DeviceWrite deviceWrite(const VXI::Req::DeviceWrite& data);
    VXI::Resp::DeviceRead deviceRead(const VXI::Req::DeviceRead& data);
};

VXI::Resp::CreateLink VXI11::Impl::createLink(const VXI::Req::CreateLink& link)
{
    socket.send(link);
    auto buffer = read();
    VXI::Resp::CreateLink resp;
    if (resp.parse(buffer, xid))
    {
        return resp;
    }
    throw std::exception("Parse CreateLink failed.");
}

VXI::Resp::DestroyLink VXI11::Impl::destroyLink(const VXI::Req::DestroyLink& link)
{
    socket.send(link);
    auto buffer = read();
    VXI::Resp::DestroyLink resp;
    if (resp.parse(buffer, xid))
    {
        return resp;
    }
    throw std::exception("Parse DestroyLink failed.");
}

VXI::Resp::DeviceWrite VXI11::Impl::deviceWrite(const VXI::Req::DeviceWrite& data)
{
    socket.send(data);
    auto buffer = read();
    VXI::Resp::DeviceWrite resp;
    if (resp.parse(buffer, xid))
    {
        return resp;
    }
    throw std::exception("Parse DeviceWrite failed.");
}

VXI::Resp::DeviceRead VXI11::Impl::deviceRead(const VXI::Req::DeviceRead& data)
{
    socket.send(data);
    auto buffer = read();
    VXI::Resp::DeviceRead resp;
    if (resp.parse(buffer, xid))
    {
        return resp;
    }
    throw std::exception("Parse DeviceRead failed.");
}

VXI11::VXI11(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>(attr)) {}

VXI11::~VXI11() {}

void VXI11::connect(const Address<AddressType::VXI11>& address,
                    const std::chrono::milliseconds& openTimeout,
                    const std::chrono::milliseconds& commandTimeout)
{
    m_impl->ioTimeout = commandTimeout;
    m_impl->socket.connect(Address<AddressType::RawSocket>(address.ip(), 111), openTimeout, commandTimeout);
    getPorts();
    m_impl->socket.close();
    m_impl->socket.connect(Address<AddressType::RawSocket>(address.ip(), m_impl->corePort), openTimeout, commandTimeout);
    VXI::Req::CreateLink link(m_impl->generateXid(), m_impl->clientId, false, 0, address.subAddress());
    auto resp = m_impl->createLink(link);
    if (resp.error() == VXI::Resp::DeviceErrorCode::NoError)
    {
        m_impl->abortPort     = resp.abortPort();
        m_impl->linkId        = resp.linkId();
        m_impl->maxBufferSize = resp.maxRecvSize();
    }
    else
    {
        throw std::exception("Connect device failed.");
    }
    m_impl->connected = true;
}

void VXI11::send(const std::string& buffer) const
{
    size_t offset { 0 };
    do
    {
        VXI::Req::DeviceWrite data(
            m_impl->generateXid(),
            m_impl->linkId,
            static_cast<unsigned long>(m_impl->ioTimeout.count()),
            0,
            std::string_view(buffer.c_str() + offset,
                             (buffer.size() - offset) > m_impl->maxBufferSize ? m_impl->maxBufferSize : buffer.size()),
            VXI::DeviceFlag { .end = buffer.size() - offset <= m_impl->maxBufferSize });

        auto resp = m_impl->deviceWrite(data);
        if (resp.error() == VXI::Resp::DeviceErrorCode::NoError)
        {
            offset += resp.size();
        }
        else
            throw std::exception("DeviceWrite failed.");
    } while (offset < buffer.size());
}

std::string VXI11::readAll() const
{
    std::string buffer;
    for (bool end { false }; !end;)
    {
        VXI::Req::DeviceRead req(m_impl->generateXid(),
                                 m_impl->linkId,
                                 static_cast<unsigned long>(m_impl->maxBufferSize),
                                 static_cast<unsigned long>(m_impl->ioTimeout.count()),
                                 0);
        auto resp = m_impl->deviceRead(req);
        if (resp.error() == VXI::Resp::DeviceErrorCode::NoError)
        {
            end = resp.reason().end;
            buffer.append(std::move(resp).data());
        }
        else
        {
            // TODO:
            throw std::exception("readAll error.");
        }
    }
    return buffer;
}

std::string VXI11::read(size_t size) const
{
    std::string buffer;
    size_t total = 0;
    for (bool end { false }; !end || total == size;)
    {
        auto reqSize = size - total > m_impl->maxBufferSize ? m_impl->maxBufferSize : size;
        VXI::Req::DeviceRead req(m_impl->generateXid(),
                                 m_impl->linkId,
                                 static_cast<unsigned long>(reqSize),
                                 static_cast<unsigned long>(m_impl->ioTimeout.count()),
                                 0);
        auto resp = m_impl->deviceRead(req);
        if (resp.error() == VXI::Resp::DeviceErrorCode::NoError)
        {
            end             = resp.reason().end;
            m_impl->avalibe = !end;
            total += resp.size();
            buffer.append(std::move(resp).data());
        }
        else
        {
            // TODO:
            throw std::exception("read error.");
        }
    }
    return buffer;
}

void VXI11::close() noexcept
{
    std::shared_ptr<int> scope(nullptr, [&](int*) { m_impl->connected = false; });
    if (m_impl->socket.connected() && m_impl->connected)
    {
        m_impl->destroyLink(VXI::Req::DestroyLink(m_impl->xid, m_impl->linkId));
    }
    m_impl->socket.close();
}

bool VXI11::connected() const noexcept { return m_impl->connected; }

size_t VXI11::avalible() const noexcept { return m_impl->avalibe; }

void VXI11::getPorts()
{
    auto func = [this](int ch) -> unsigned short
    {
        RPC::Req::GetPort<Proto::TCP> call(m_impl->generateXid(), ch, 1, Proto::TCP);
        m_impl->socket.send(call);

        RPC::Resp::GetPort<Proto::TCP> port;
        auto buffer = m_impl->read();
        bool ret    = port.parse(buffer, m_impl->xid);
        if (ret)
            return port.port();
        else
            throw std::exception("Port Map failed");
    };

    m_impl->corePort = func(CoreChannel);
}

} // namespace OpenVisa
