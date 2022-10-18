#include "HiSLIP.h"
#include "RawSocket.h"

namespace OpenVisa
{
constexpr size_t SendMax = 1024;

inline void throwFatalError(HS::FatalErrorCode code)
{
    switch (code)
    {
    case OpenVisa::HS::FatalErrorCode::Unknown:
        throw std::exception("Unidentified error.");
    case OpenVisa::HS::FatalErrorCode::MessageHeaderError:
        throw std::exception("Poorly formed message header.");
    case OpenVisa::HS::FatalErrorCode::CommunicationWithoutBothChannel:
        throw std::exception("Attempt to use connection without both channels established.");
    case OpenVisa::HS::FatalErrorCode::InvalidInitSequence:
        throw std::exception("Invalid Initialization Sequence.");
    case OpenVisa::HS::FatalErrorCode::ServerRefusedWithMaximumClients:
        throw std::exception("Server refused connection due to maximum number of clients exceeded.");
    case OpenVisa::HS::FatalErrorCode::SecureConnectionFailed:
        throw std::exception("Secure connection failed.");
    default:
        throw std::exception("Unknown Fatal Error Code.");
    }
}

inline void throwError(HS::ErrorCode code)
{
    switch (code)
    {
    case HS::ErrorCode::Unknown:
        throw std::exception("Unidentified error.");
    case HS::ErrorCode::UnrecognizedMessageType:
        throw std::exception("Unrecognized Message Type.");
    case HS::ErrorCode::UnrecognizedControlCode:
        throw std::exception("Unrecognized control code.");
    case HS::ErrorCode::UnrecognizedVendorDefinedMessage:
        throw std::exception("Unrecognized Vendor Defined Message.");
    case HS::ErrorCode::MessageTooLarge:
        throw std::exception("Message too large.");
    case HS::ErrorCode::AuthenticationFailed:
        throw std::exception("Authentication failed.");
    default:
        throw std::exception("Unknown Error Code.");
    }
}

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
};

void HiSLIP::initialize(std::string_view subAddr)
{
    HS::Req::Initialize req(0x0100, 0x00, subAddr);
    m_impl->socket.send(req);
    HS::HSBuffer buffer;
    HS::Resp::Initialize resp;
    do
    {
        buffer.std::string::append(m_impl->read());
    } while (!buffer.isEnough());
    errorCheck(buffer);
    resp.parse(buffer);
    m_impl->sessionId = resp.sessionId();
}

void HiSLIP::initializeAsync()
{
    HS::Req::AsyncInitialize req(m_impl->sessionId);
    m_impl->asyncSocket.send(req);
    HS::HSBuffer buffer;
    do
    {
        buffer.std::string::append(m_impl->readAsync());
    } while (!buffer.isEnough());
    errorCheck(buffer);
    HS::Resp::AsyncInitialize resp;
    resp.parse(buffer);
}

void HiSLIP::errorCheck(HS::HSBuffer& buffer) const
{
    if (!buffer.isVaild())
        throw std::exception("Error hislip data.");
    switch (buffer.messageType())
    {
    case HS::MessageType::FatalError:
        {
            const_cast<HiSLIP*>(this)->close();
            HS::Resp::FatalError resp;
            resp.parse(buffer);
            throwFatalError(resp.code());
        }
        break;
    case HS::MessageType::Error:
        {
            HS::Resp::Error resp;
            resp.parse(buffer);
            throwError(resp.code());
        }
    }
}

HiSLIP::HiSLIP(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>(attr)) {}

HiSLIP::~HiSLIP() {}

void HiSLIP::connect(const Address<AddressType::HiSLIP>& address, const std::chrono::milliseconds& openTimeout)
{
    m_impl->socket.connect(Address<AddressType::RawSocket>(address.ip(), address.port()), openTimeout);
    initialize(address.subAddress());
    m_impl->asyncSocket.connect(Address<AddressType::RawSocket>(address.ip(), address.port()), openTimeout);
    initializeAsync();
    m_impl->connected = true;
}

void HiSLIP::send(const std::string& buffer) const
{
    size_t offset = 0;
    for (;;)
    {
        if (offset + SendMax < buffer.size() - offset)
        {
            HS::Req::Data req(buffer.substr(offset, SendMax), m_impl->msgId);
            m_impl->socket.send(req);
            offset += SendMax;
        }
        else
        {
            HS::Req::DataEnd req(buffer.substr(offset, buffer.size() - offset), m_impl->msgId);
            m_impl->msgId += 2;
            m_impl->socket.send(req);
            return;
        }
    }
}

std::string HiSLIP::readAll() const
{
    HS::HSBuffer buffer;
    std::string data;
    for (;;)
    {
        buffer.std::string::append(m_impl->read());
        while (buffer.isEnough())
        {
            errorCheck(buffer);
            switch (buffer.messageType())
            {
            case HS::MessageType::Data:
                {
                    HS::Resp::Data resp;
                    resp.parse(buffer);
                    data.append(resp.data());
                    break;
                }
            case HS::MessageType::DataEnd:
                {
                    HS::Resp::DataEnd resp;
                    resp.parse(buffer);
                    data.append(resp.data());
                    return data;
                }
            }
        }
    }
}

std::string HiSLIP::read(size_t size) const { return readAll(); }

void HiSLIP::close() noexcept
{
    if (m_impl->connected)
    {
        m_impl->connected = false;
        m_impl->socket.close();
        m_impl->asyncSocket.close();
    }
}

bool HiSLIP::connected() const noexcept { return m_impl->connected && m_impl->socket.connected() && m_impl->asyncSocket.connected(); }

size_t HiSLIP::avalible() const noexcept { return {}; }

} // namespace OpenVisa