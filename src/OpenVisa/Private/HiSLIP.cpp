#include "HiSLIP.h"
#include "HiSLIPProtocol.h"
#include "RawSocket.h"

namespace OpenVisa
{
struct HiSLIP::Impl
{
    RawSocket socket;
    RawSocket asyncSocket;
    bool connected { false };
    unsigned short sessionId { 0 };
    unsigned int msgId { 0xFFFFFF00 };
    inline Impl(Object::Attribute const& attr) : socket(attr), asyncSocket(attr) {}
    inline HS::HSBuffer read()
    {
        HS::HSBuffer buffer;
        do
        {
            buffer.std::string::append(socket.read(1024));
        } while (socket.avalible());
        return buffer;
    }
    inline HS::HSBuffer readAsync()
    {
        HS::HSBuffer buffer;
        do
        {
            buffer.std::string::append(asyncSocket.read(1024));
        } while (asyncSocket.avalible());
        return buffer;
    }
    inline void initialize(std::string_view subAddr);
    inline void initializeAsync();
};

inline void HiSLIP::Impl::initialize(std::string_view subAddr)
{
    HS::Req::Initialize req(0x0100, 0x00, subAddr);
    socket.send(req);
    HS::HSBuffer buffer;
    HS::Resp::Initialize resp;
    do
    {
        buffer.std::string::append(read());
    } while (!resp.parse(buffer));
    sessionId = resp.sessionId();
}

void HiSLIP::Impl::initializeAsync()
{
    HS::Req::AsyncInitialize req(sessionId);
    asyncSocket.send(req);
    HS::HSBuffer buffer;
    HS::Resp::AsyncInitialize resp;
    do
    {
        buffer.std::string::append(readAsync());
    } while (!resp.parse(buffer));
}

HiSLIP::HiSLIP(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>(attr)) {}

HiSLIP::~HiSLIP() {}

void HiSLIP::connect(const Address<AddressType::HiSLIP>& address,
                     const std::chrono::milliseconds& openTimeout,
                     const std::chrono::milliseconds& commandTimeout)
{
    m_impl->socket.connect(Address<AddressType::RawSocket>(address.ip(), address.port()), openTimeout, commandTimeout);
    m_impl->initialize("hislip0");
    m_impl->asyncSocket.connect(Address<AddressType::RawSocket>(address.ip(), address.port()), openTimeout, commandTimeout);
    m_impl->initializeAsync();
    m_impl->connected = true;
}

void HiSLIP::send(const std::string& buffer) const
{
    HS::Req::DataEnd req(buffer, m_impl->msgId);
    m_impl->msgId += 2;
    m_impl->socket.send(req);
}

std::string HiSLIP::readAll() const
{
    auto buffer = m_impl->read();
    HS::Resp::DataEnd resp;
    if (resp.parse(buffer))
    {
        return resp.data();
    }
    return {};
}

std::string HiSLIP::read(size_t size) const { return {}; }

void HiSLIP::close() noexcept { m_impl->connected = false; }

bool HiSLIP::connected() const noexcept { return m_impl->connected && m_impl->socket.connected(); }

size_t HiSLIP::avalible() const noexcept { return {}; }

} // namespace OpenVisa