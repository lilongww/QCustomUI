#pragma once
#include "OncRpcProtocol.h"

namespace OpenVisa
{
inline namespace VXI
{
#pragma pack(push)
#pragma pack(1)
enum class Proc
{
    CreateLink      = 10,
    DeviceWrite     = 11,
    DeviceRead      = 12,
    DeviceReadSTB   = 13,
    DeviceTrigger   = 14,
    DeviceClear     = 15,
    DeviceRemote    = 16,
    DeviceLocal     = 17,
    DeviceLock      = 18,
    DeviceUnlock    = 19,
    DeviceEnableSrq = 20,
    DeviceDoCmd     = 22,
    DestroyLink     = 23,
    CreateIntrChan  = 25,
    DestoryIntrChan = 26
};
using DeviceLink = long;

struct DeviceFlag
{
    long waitLock   : 1 { false };
    long reserved1  : 2 { 0 };
    long end        : 1 { 0 };
    long reserved2  : 3 { 0 };
    long termchrset : 1 { false };
    long reserved3  : 24 { 0 };
};

static_assert(sizeof(DeviceFlag) == 4);

struct DeviceReason
{
    long reqcnt : 1;
    long chr    : 1;
    long end    : 1;
    long reserved : sizeof(long) - 3;
};

static_assert(sizeof(DeviceReason) == 4);

constexpr auto CoreChannel      = 395183;
constexpr auto AbortChannel     = 395184;
constexpr auto InterruptChannel = 395185;

namespace Req
{
class CreateLink : public RPC::Req::RpcCall<RPC::Proto::TCP>
{
public:
    inline CreateLink(unsigned int xid, long id, bool lock, unsigned long lockTimeout, std::string_view name) noexcept
        : RPC::Req::RpcCall<RPC::Proto::TCP>(xid, CoreChannel, 1, static_cast<unsigned int>(Proc::CreateLink))
        , m_clientID(id)
        , m_lockDevice(lock)
        , m_lockTimeout(lockTimeout)
        , m_name(name)
    {
    }

    operator RPCBuffer() const override
    {
        auto buffer = RPC::Req::RpcCall<RPC::Proto::TCP>::operator OpenVisa::RPC::RPCBuffer();
        buffer.append(m_clientID);
        buffer.append(m_lockDevice);
        buffer.append(m_lockTimeout);
        buffer.append(static_cast<unsigned long>(m_name.size()));
        if (!m_name.empty())
        {
            buffer.std::string::append(m_name);
            if (auto r = m_name.size() % 4; r)
                buffer.resize(buffer.size() + (4 - r));
        }
        setSizeByte(buffer);
        return buffer;
    }

private:
    long m_clientID;             /* implementation specific value.*/
    long m_lockDevice;           /* attempt to lock the device */
    unsigned long m_lockTimeout; /* time to wait on a lock */
    std::string m_name;          /* name of device */
};

class DestroyLink : public RPC::Req::RpcCall<Proto::TCP>
{
public:
    inline DestroyLink(unsigned int xid, DeviceLink id) noexcept
        : RPC::Req::RpcCall<Proto::TCP>(xid, CoreChannel, 1, static_cast<unsigned int>(Proc::DestroyLink)), m_linkId(id)
    {
    }

    operator RPCBuffer() const override
    {
        auto buffer = RPC::Req::RpcCall<Proto::TCP>::operator OpenVisa::RPC::RPCBuffer();
        buffer.append(m_linkId);
        setSizeByte(buffer);
        return buffer;
    }

private:
    DeviceLink m_linkId;
};

class DeviceWrite : public RPC::Req::RpcCall<RPC::Proto::TCP>
{
public:
    inline DeviceWrite(unsigned int xid,
                       DeviceLink linkId,
                       unsigned long ioTimeout,
                       unsigned long lockTimeout,
                       std::string_view data,
                       DeviceFlag flags = {})
        : RPC::Req::RpcCall<RPC::Proto::TCP>(xid, CoreChannel, 1, static_cast<unsigned int>(Proc::DeviceWrite))
        , m_linkId(linkId)
        , m_ioTimeout(ioTimeout)
        , m_lockTimeout(lockTimeout)
        , m_flags(flags)
        , m_data(data)
    {
    }

    operator RPCBuffer() const override
    {
        auto buffer = RPC::Req::RpcCall<RPC::Proto::TCP>::operator OpenVisa::RPC::RPCBuffer();
        buffer.append(m_linkId);
        buffer.append(m_ioTimeout);
        buffer.append(m_lockTimeout);
        buffer.append(*reinterpret_cast<const long*>(&m_flags));
        buffer.append(static_cast<unsigned int>(m_data.size()));
        if (!m_data.empty())
        {
            buffer.std::string::append(m_data.begin(), m_data.end());
            if (auto ret = m_data.size() % 4; ret)
                buffer.resize(buffer.size() + (4 - ret));
        }
        setSizeByte(buffer);
        return buffer;
    }

private:
    DeviceLink m_linkId;
    unsigned long m_ioTimeout;
    unsigned long m_lockTimeout;
    DeviceFlag m_flags;
    std::string_view m_data;
};

class DeviceRead : public RPC::Req::RpcCall<RPC::Proto::TCP>
{
public:
    DeviceRead(unsigned int xid,
               DeviceLink linkId,
               unsigned long requestSize,
               unsigned long ioTimeout,
               unsigned long lockTimeout,
               DeviceFlag flags = {},
               char termChar    = '0' /* valid if flags & termchrset */)
        : RPC::Req::RpcCall<RPC::Proto::TCP>(xid, CoreChannel, 1, static_cast<unsigned int>(Proc::DeviceRead))
        , m_linkId(linkId)
        , m_requestSize(requestSize)
        , m_ioTimeout(ioTimeout)
        , m_lockTimeout(lockTimeout)
        , m_flags(flags)
        , m_termChar(termChar)
    {
    }

    operator RPCBuffer() const override
    {
        auto buffer = RPC::Req::RpcCall<RPC::Proto::TCP>::operator OpenVisa::RPC::RPCBuffer();
        buffer.append(m_linkId);
        buffer.append(m_requestSize);
        buffer.append(m_ioTimeout);
        buffer.append(m_lockTimeout);
        buffer.append(*reinterpret_cast<const unsigned long*>(&m_flags));
        buffer.append(m_termChar);
        setSizeByte(buffer);
        return buffer;
    }

private:
    DeviceLink m_linkId;
    unsigned long m_requestSize;
    unsigned long m_ioTimeout;
    unsigned long m_lockTimeout;
    DeviceFlag m_flags;
    unsigned long m_termChar;
};
} // namespace Req

namespace Resp
{
enum class DeviceErrorCode : long
{
    NoError                   = 0,
    SyntaxError               = 1,
    NotAccessible             = 3,
    InvalidLinkId             = 4,
    ParamError                = 5,
    ChannelNotEstablished     = 6,
    OperationNotSupported     = 8,
    OutOfResources            = 9,
    DeviceLocked              = 11,
    NoLock                    = 12,
    Timeout                   = 15,
    IOError                   = 17,
    InvalidAddress            = 21,
    Abort                     = 23,
    ChannelAlreadyEstablished = 29
};

class CreateLink : public RPC::Resp::RpcReply<Proto::TCP>
{
public:
    bool parse(RPCBuffer& buffer, unsigned int xid) override
    {
        bool ret = RPC::Resp::RpcReply<Proto::TCP>::parse(buffer, xid);
        if (ret)
        {
            auto buffer = this->buffer();
            buffer.take(m_error);
            buffer.take(m_lid);
            buffer.take(m_abortPort);
            buffer.take(m_maxRecvSize);
        }
        return ret;
    }
    inline DeviceErrorCode error() const noexcept { return m_error; }
    inline DeviceLink linkId() const noexcept { return m_lid; }
    inline unsigned short abortPort() const noexcept { return static_cast<unsigned short>(m_abortPort); }
    inline unsigned long maxRecvSize() const noexcept { return m_maxRecvSize; }

private:
    DeviceErrorCode m_error;
    DeviceLink m_lid;
    unsigned long m_abortPort;   /* for the abort RPC */
    unsigned long m_maxRecvSize; /* specifies max data size in bytes
    device will accept on a write */
};

class DestroyLink : public RPC::Resp::RpcReply<Proto::TCP>
{
public:
    bool parse(RPCBuffer& buffer, unsigned int xid) override
    {
        auto ret = RPC::Resp::RpcReply<Proto::TCP>::parse(buffer, xid);
        if (ret)
        {
            auto buffer = this->buffer();
            buffer.take(m_error);
        }
        return ret;
    }
    inline DeviceErrorCode error() const noexcept { return m_error; }

private:
    DeviceErrorCode m_error;
};

class DeviceWrite : public RPC::Resp::RpcReply<RPC::Proto::TCP>
{
public:
    bool parse(RPCBuffer& buffer, unsigned int xid) override
    {
        auto ret = RPC::Resp::RpcReply<RPC::Proto::TCP>::parse(buffer, xid);
        if (ret)
        {
            auto buffer = this->buffer();
            buffer.take(m_error);
            buffer.take(m_size);
        }
        return ret;
    }
    inline DeviceErrorCode error() const noexcept { return m_error; }
    inline unsigned long size() const noexcept { return m_size; }

private:
    DeviceErrorCode m_error;
    unsigned long m_size;
};

class DeviceRead : public RPC::Resp::RpcReply<RPC::Proto::TCP>
{
public:
    bool parse(RPCBuffer& buffer, unsigned int xid) override
    {
        auto ret = RPC::Resp::RpcReply<RPC::Proto::TCP>::parse(buffer, xid);
        if (ret)
        {
            auto buffer = this->buffer();
            buffer.take(m_error);
            buffer.take(reinterpret_cast<long&>(m_reason));
            unsigned long size;
            buffer.take(size);
            m_buffer = std::string(buffer.begin(), buffer.begin() + size);
        }
        return ret;
    }
    inline DeviceErrorCode error() const noexcept { return m_error; }
    inline DeviceReason reason() const noexcept { return m_reason; }
    inline std::string data() && noexcept { return std::move(m_buffer); }
    inline size_t size() const noexcept { return m_buffer.size(); }

private:
    DeviceErrorCode m_error;
    DeviceReason m_reason;
    std::string m_buffer;
};
} // namespace Resp
#pragma pack(pop)
} // namespace VXI
} // namespace OpenVisa
