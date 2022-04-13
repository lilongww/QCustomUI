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
constexpr unsigned SizeHeaderMask  = std::endian::native == std::endian::big ? 0x80'00'00'00 : 0x00'00'00'80;
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

        setSizeByte(buffer);
        return buffer;
    }

    static inline void setSizeByte(RPCBuffer& buffer)
    {
        if constexpr (type == Proto::TCP)
        { // TCP首先发送负字节数
            *reinterpret_cast<int*>(buffer.data()) = toBigEndian<int>((static_cast<int>(buffer.size()) - 4)) | SizeHeaderMask;
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
        RpcCall<type>::setSizeByte(buffer);
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
    virtual bool parse(RPCBuffer& buffer, unsigned int xid)
    {
        size_t total = 0;
        if (type == Proto::TCP)
        {
            if (buffer.size() < 4)
                return false;
            auto tmp = *reinterpret_cast<int*>(buffer.data()) & ~SizeHeaderMask;
            total    = fromBigEndian(tmp);
            if (buffer.size() < total + 4)
                return false;
            buffer.erase(buffer.begin(), buffer.begin() + 4);
        }
        else
            total = buffer.size();
        auto size = buffer.take(m_xid);
        size += buffer.take(m_type);
        size += buffer.take(m_replyStat);
        switch (m_replyStat)
        {
        case OpenVisa::RPC::ReplyStat::Accept:
            {
                auto& accept = std::get<0>(m_reply);
                size += buffer.take(accept.ar_verf);
                size += buffer.take(accept.ar_stat);
                switch (accept.ar_stat)
                {
                case AcceptState::Success:
                    m_buffer                  = RPCBuffer(buffer.begin(), buffer.begin() + (total - size));
                    accept.ru.AR_results.wh   = buffer.data();
                    accept.ru.AR_results.size = static_cast<unsigned int>(buffer.size());
                    buffer.erase(buffer.begin(), buffer.begin() + (total - size));
                    return true;
                case AcceptState::ProgMismatch:
                    size += buffer.take(accept.ru.AR_versions.high);
                    size += buffer.take(accept.ru.AR_versions.low);
                    break;
                }
            }
            break;
        case OpenVisa::RPC::ReplyStat::Denied:
            {
                m_reply      = rejected_reply {};
                auto& reject = std::get<1>(m_reply);
                size += buffer.take(reject.rj_stat);
                switch (reject.rj_stat)
                {
                case RejectState::AuthError:
                    size += buffer.take(reject.ru.RJ_why);
                    break;
                case RejectState::RpcMismatch:
                    size += buffer.take(reject.ru.RJ_versions.high);
                    size += buffer.take(reject.ru.RJ_versions.low);
                    break;
                }
            }
            break;
        default:
            break;
        }
        if (total != size)
        {
            buffer.erase(buffer.begin(), buffer.begin() + total - size);
        }
        return false;
    }

    inline const RPCBuffer& buffer() const noexcept { return m_buffer; }

protected:
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
    bool parse(RPCBuffer& buffer, unsigned int xid) override
    {
        auto ret = RpcReply<type>::parse(buffer, xid);
        if (ret)
            m_port = fromBigEndian(*reinterpret_cast<const unsigned int*>(RpcReply<type>::buffer().c_str()));
        return ret;
    }

private:
    unsigned short m_port;
};
} // namespace Resp
#pragma pack(pop)
} // namespace RPC
} // namespace OpenVisa
