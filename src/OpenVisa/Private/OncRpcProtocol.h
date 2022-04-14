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
#pragma once

#include "Endian.h"

#include <type_traits>
#include <variant>

namespace OpenVisa
{
inline namespace RPC
{
// RFC 1057
#pragma pack(push)
#pragma pack(1)

constexpr auto MAX_AUTH_BYTES      = 400;
constexpr unsigned int PortMapProg = 100'000;

struct TcpHeader
{
    unsigned int size         : 31;
    unsigned int lastFragment : 1;
};
static_assert(sizeof(TcpHeader) == 4);

enum class PortMapProc : unsigned int
{
    Null,    // void (void)
    Set,     // bool (mapping)
    Unset,   // bool (mapping)
    GetPort, // unsigned int (mapping)
    Dump,    // pmaplist (void)
    CallIt   // call_result (call_args)
};

enum class Proto : unsigned int
{
    TCP = 6,
    UDP = 17
};

struct Mapping
{
    unsigned int prog { 0 };
    unsigned int vers { 0 };
    Proto prot { 0 };
    unsigned int port { 0 };
};

struct Maplist
{
    Mapping map;
    Maplist* next { nullptr };
};

class OpaqueAuth;
class RPCBuffer : public std::string
{
public:
    using std::string::basic_string;
    template<typename T>
    requires std::is_arithmetic_v<T> || std::is_enum_v<T>
    inline void append(const T bytes)
    {
        auto val = toBigEndian(bytes);
        std::string::append(reinterpret_cast<const char*>(&val), sizeof(T));
    }
    template<typename T>
    requires std::is_arithmetic_v<T> || std::is_enum_v<T>
    inline size_t take(T& val)
    {
        val = fromBigEndian(*reinterpret_cast<T*>(data()));
        erase(begin(), begin() + sizeof(T));
        return sizeof(T);
    }
    inline size_t take(OpaqueAuth& val);
    // 仅TCP
    inline bool isEnough() const noexcept
    {
        if (this->size() < 4)
            return false;
        size_t offset { 0 };
        for (;;)
        {
            auto header = headerOf(c_str() + offset);
            if (header.lastFragment)
                return offset + header.size + 4 <= size();
            else if (size() >= offset + header.size + 4)
                offset += header.size + 4;
            else
                return false;
        }
    }
    inline TcpHeader headerSize() const noexcept { return headerOf(c_str()); }

private:
    inline TcpHeader headerOf(const char* ptr) const
    {
        auto tmp = fromBigEndian(*reinterpret_cast<const unsigned int*>(ptr));
        return reinterpret_cast<TcpHeader&>(tmp);
    }
};

enum class AuthFlavor : unsigned int
{
    Null  = 0,
    Unix  = 1,
    Short = 2,
    Des   = 3
};

class OpaqueAuth
{
public:
    inline OpaqueAuth() noexcept {}
    inline OpaqueAuth(AuthFlavor au, const std::string& op = {}) : m_auth(au), m_opaque(op) {}
    inline OpaqueAuth(const OpaqueAuth& rhs) noexcept
    {
        m_auth   = rhs.m_auth;
        m_opaque = rhs.m_opaque;
    }
    inline OpaqueAuth(OpaqueAuth&& rhs) noexcept
    {
        m_auth = rhs.m_auth;
        std::swap(m_opaque, rhs.m_opaque);
    }
    inline const std::string& opaque() const noexcept { return m_opaque; }
    inline operator std::string() const
    {
        RPCBuffer buffer;
        buffer.append(m_auth);
        buffer.append(static_cast<unsigned int>(m_opaque.size()));
        if (!m_opaque.empty())
            buffer += m_opaque;
        return buffer;
    }

private:
    AuthFlavor m_auth { AuthFlavor::Null };
    std::string m_opaque;
    friend RPCBuffer;
};

inline size_t RPCBuffer::take(OpaqueAuth& val)
{
    auto ret = this->take(val.m_auth);
    unsigned int size;
    ret += take(size);
    if (size)
    {
        val.m_opaque = this->substr(0, size);
        erase(0, size);
        ret += size;
    }
    return ret;
}

enum class MsgType
{
    Call  = 0,
    Reply = 1
};

enum class ReplyStat
{
    Accept = 0,
    Denied = 1
};

enum class AcceptState
{
    Success       = 0, /* RPC成功执行      */
    ProgUnavailbe = 1, /* 远程没有输出过程  */
    ProgMismatch  = 2, /* 不支持远程版本 #  */
    ProcUnavailbe = 3, /* 程序不支持远远程过程 */
    GarbageArgs   = 4  /* 过程不能解参数   */
};

enum class RejectState
{
    RpcMismatch = 0, /* RPC版本!=2          */
    AuthError   = 1  /* Romote不能鉴定调用者 */
};

enum class AuthStat
{
    BadCard        = 1, /* 坏信任书 (坏的签名) */
    RejectedRedist = 2, /* 客户必须重新调用 */
    BadVerf        = 3, /* 错误校验(签名破坏) */
    RejectedVerf   = 4, /* 验证口令期满或破坏  */
    TooWeak        = 5  /* 安全原因拒绝 */
};

struct accepted_reply
{
    accepted_reply() {}
    ~accepted_reply() {}
    OpaqueAuth ar_verf;
    AcceptState ar_stat;
    union
    {
        // mismatch
        struct
        {
            unsigned int low;
            unsigned int high;
        } AR_versions;
        // success
        struct
        {
            char* wh;
            unsigned int size;
        } AR_results;
    } ru;
};

struct rejected_reply
{
    RejectState rj_stat;
    union
    {
        struct
        {
            unsigned int low;
            unsigned int high;
        } RJ_versions;
        AuthStat RJ_why; /* why authentication did not work */
    } ru;
};

// struct rpc_msg
//{
//     unsigned long rm_xid;
//     MsgType rm_direction;
//     union
//     {
//         call_body RM_cmb;
//         reply_body RM_rmb;
//     } ru;
// };
namespace Req
{

// 描述当前是使用TCP通信还是UDP通信
template<Proto type>
class RpcCall
{
public:
    inline RpcCall(unsigned int xid,
                   unsigned int prog,
                   unsigned int vers,
                   unsigned int proc,
                   const OpaqueAuth& cred = {},
                   const OpaqueAuth& verf = {})
        : m_xid(xid), m_prog(prog), m_vers(vers), m_proc(proc), m_verf(verf)
    {
    }
    inline ~RpcCall() {}

    virtual operator RPCBuffer() const
    {
        RPCBuffer buffer;
        if constexpr (type == Proto::TCP)
        {
            buffer.resize(4);
        }

        buffer.append(m_xid);
        buffer.append(m_type);
        buffer.append(m_rpcvers);
        buffer.append(m_prog);
        buffer.append(m_vers);
        buffer.append(m_proc);
        buffer += m_cred;
        buffer += m_verf;

        setSizeByte(buffer, true);
        return buffer;
    }

    static inline void setSizeByte(RPCBuffer& buffer, bool lastFragment)
    {
        if constexpr (type == Proto::TCP)
        { // TCP首先发送负字节数
            TcpHeader header;
            header.lastFragment                    = lastFragment;
            header.size                            = buffer.size() - 4;
            *reinterpret_cast<int*>(buffer.data()) = toBigEndian<int>(*reinterpret_cast<int*>(&header));
        }
    }

private:
    unsigned int m_xid;
    MsgType m_type { MsgType::Call };
    unsigned int m_rpcvers { 2 }; /* must be equal to two */
    unsigned int m_prog;
    unsigned int m_vers;
    unsigned int m_proc;
    OpaqueAuth m_cred;
    OpaqueAuth m_verf; /* protocol specific - provided by client */
};

template<Proto type>
class GetPort : public RpcCall<type>
{
public:
    // 这里的prot描述的是要获取的端口号是UDP还是TCP协议的。
    inline GetPort(unsigned int xid, unsigned int prog, unsigned int vers, Proto prot)
        : RpcCall<type>(xid, RPC::PortMapProg, 2, static_cast<unsigned int>(RPC::PortMapProc::GetPort))
    {
        m_mapping.prog = prog;
        m_mapping.vers = vers;
        m_mapping.prot = prot;
        m_mapping.port = 0;
    }
    inline ~GetPort() {}
    operator RPCBuffer() const override
    {
        RPCBuffer buffer(RpcCall<type>::operator RPCBuffer());
        buffer.append(m_mapping.prog);
        buffer.append(m_mapping.vers);
        buffer.append(m_mapping.prot);
        buffer.append(m_mapping.port);
        RpcCall<type>::setSizeByte(buffer, true);
        return buffer;
    }

private:
    Mapping m_mapping;
};
} // namespace Req

namespace Resp
{
template<Proto type>
class RpcReply
{
public:
    inline RpcReply() {}
    inline ~RpcReply() {}
    virtual void parse(RPCBuffer& buffer, unsigned int xid)
    {
        unsigned int size { 0 };
        bool lastFragment { false };
        if (type == Proto::TCP)
        {
            size         = buffer.headerSize().size;
            lastFragment = buffer.headerSize().lastFragment;
            buffer.erase(buffer.begin(), buffer.begin() + 4);
        }
        else
            size = static_cast<unsigned int>(buffer.size());
        auto offset = buffer.take(m_xid);
        offset += buffer.take(m_type);
        offset += buffer.take(m_replyStat);

        switch (m_replyStat)
        {
        case OpenVisa::RPC::ReplyStat::Accept:
            {
                auto& accept = std::get<0>(m_reply);
                offset += buffer.take(accept.ar_verf);
                offset += buffer.take(accept.ar_stat);
                switch (accept.ar_stat)
                {
                case AcceptState::Success:
                    {
                        m_dataSize                = static_cast<unsigned int>(size - offset);
                        m_buffer                  = RPCBuffer(buffer.begin(), buffer.begin() + m_dataSize);
                        accept.ru.AR_results.wh   = buffer.data();
                        accept.ru.AR_results.size = static_cast<unsigned int>(buffer.size());
                        buffer.erase(buffer.begin(), buffer.begin() + m_dataSize);
                        while (!lastFragment)
                        {
                            auto header = buffer.headerSize();
                            m_buffer.std::string::append(buffer.begin() + 4, buffer.begin() + header.size + 4);
                            buffer.erase(buffer.begin(), buffer.begin() + 4 + header.size);
                            lastFragment = header.lastFragment;
                        }
                        return;
                    }
                case AcceptState::ProgUnavailbe:
                    throw std::exception("remote hasn't exported program.");
                    break;
                case AcceptState::ProgMismatch:
                    offset += buffer.take(accept.ru.AR_versions.high);
                    offset += buffer.take(accept.ru.AR_versions.low);
                    throw std::exception(std::format("remote can't support version, minimum version: {}, maximum version: {}.",
                                                     accept.ru.AR_versions.low,
                                                     accept.ru.AR_versions.high)
                                             .c_str());
                case AcceptState::ProcUnavailbe:
                    throw std::exception("program can't support procedure.");
                case AcceptState::GarbageArgs:
                    throw std::exception("procedure can't decode params.");
                default:
                    throw std::exception("unknown OpenVisa::RPC::AcceptState.");
                }
            }
            break;
        case OpenVisa::RPC::ReplyStat::Denied:
            {
                m_reply      = rejected_reply {};
                auto& reject = std::get<1>(m_reply);
                offset += buffer.take(reject.rj_stat);
                switch (reject.rj_stat)
                {
                case RejectState::AuthError:
                    offset += buffer.take(reject.ru.RJ_why);
                    throw std::exception("remote can't authenticate caller.");
                    break;
                case RejectState::RpcMismatch:
                    offset += buffer.take(reject.ru.RJ_versions.high);
                    offset += buffer.take(reject.ru.RJ_versions.low);
                    throw std::exception("RPC version number != 2.");
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    inline const RPCBuffer& buffer() const noexcept { return m_buffer; }

protected:
    unsigned int m_dataSize { 0 };
    unsigned int m_xid;
    MsgType m_type;
    ReplyStat m_replyStat;
    std::variant<accepted_reply, rejected_reply> m_reply;

    RPCBuffer m_buffer;
};

template<Proto type>
class GetPort : public RpcReply<type>
{
public:
    inline unsigned short port() const { return m_port; }
    void parse(RPCBuffer& buffer, unsigned int xid) override
    {
        RpcReply<type>::parse(buffer, xid);
        m_port = fromBigEndian(*reinterpret_cast<const unsigned int*>(RpcReply<type>::buffer().c_str()));
    }

private:
    unsigned short m_port;
};
} // namespace Resp
#pragma pack(pop)
} // namespace RPC
} // namespace OpenVisa
