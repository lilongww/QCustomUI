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

namespace OpenVisa
{
constexpr uint8_t USBTMC_MSGID_DEV_DEP_MSG_OUT               = 1;
constexpr uint8_t USBTMC_MSGID_REQUEST_DEV_DEP_MSG_IN        = 2;
constexpr uint8_t USBTMC_MSGID_DEV_DEP_MSG_IN                = 2;
constexpr uint8_t USBTMC_MSGID_VENDOR_SPECIFIC_OUT           = 126;
constexpr uint8_t USBTMC_MSGID_REQUEST_VENDOR_SPECIFIC_IN    = 127;
constexpr uint8_t USBTMC_MSGID_VENDOR_SPECIFIC_IN            = 127;
constexpr uint8_t USB488_MSGID_TRIGGER                       = 128;
constexpr uint8_t USBTMC_STATUS_SUCCESS                      = 0x01;
constexpr uint8_t USBTMC_STATUS_PENDING                      = 0x02;
constexpr uint8_t USBTMC_STATUS_FAILED                       = 0x80;
constexpr uint8_t USBTMC_STATUS_TRANSFER_NOT_IN_PROGRESS     = 0x81;
constexpr uint8_t USBTMC_STATUS_SPLIT_NOT_IN_PROGRESS        = 0x82;
constexpr uint8_t USBTMC_STATUS_SPLIT_IN_PROGRESS            = 0x83;
constexpr uint8_t USB488_STATUS_INTERRUPT_IN_BUSY            = 0x20;
constexpr uint8_t USBTMC_REQUEST_INITIATE_ABORT_BULK_OUT     = 1;
constexpr uint8_t USBTMC_REQUEST_CHECK_ABORT_BULK_OUT_STATUS = 2;
constexpr uint8_t USBTMC_REQUEST_INITIATE_ABORT_BULK_IN      = 3;
constexpr uint8_t USBTMC_REQUEST_CHECK_ABORT_BULK_IN_STATUS  = 4;
constexpr uint8_t USBTMC_REQUEST_INITIATE_CLEAR              = 5;
constexpr uint8_t USBTMC_REQUEST_CHECK_CLEAR_STATUS          = 6;
constexpr uint8_t USBTMC_REQUEST_GET_CAPABILITIES            = 7;
constexpr uint8_t USBTMC_REQUEST_INDICATOR_PULSE             = 64;
constexpr uint8_t USB488_READ_STATUS_BYTE                    = 128;
constexpr uint8_t USB488_REN_CONTROL                         = 160;
constexpr uint8_t USB488_GOTO_LOCAL                          = 161;
constexpr uint8_t USB488_LOCAL_LOCKOUT                       = 162;

class BulkOut
{
public:
#pragma pack(push)
#pragma pack(1)
    class Header
    {
    public:
        inline Header(unsigned char msgId, unsigned char tag, unsigned int size, bool lastPack)
            : m_msgId(msgId), m_tag(tag), m_tagInverse(~tag), m_transferSize(size), m_bmTransfer(lastPack)
        {
        }
        inline operator std::string() const { return std::string(reinterpret_cast<const char*>(this), sizeof(*this)); }

    private:
        unsigned char m_msgId;
        unsigned char m_tag;                      // 每次发送增加1, 1<=tag<=255
        unsigned char m_tagInverse;               // m_tag的补码
        unsigned char m_reserved1 { 0x00 };       // 必须为0x00
        unsigned int m_transferSize;              // 传输数据大小
        bool m_bmTransfer;                        // EOM 是否最后一包
        unsigned char m_reserved2[3] { 0, 0, 0 }; // 填充字节
    };
#pragma pack(pop)

    inline BulkOut(unsigned char& tag, unsigned char msgId, unsigned short maxPacketSize) noexcept
        : m_refTag(tag), m_msgId(msgId), m_maxPacketSize(maxPacketSize)
    {
    }
    inline ~BulkOut() noexcept {}
    inline void append(std::string&& buffer) { m_buffer.append(buffer); }
    inline void append(const std::string& buffer) { m_buffer.append(buffer); }

    inline operator std::vector<std::string>() noexcept
    {
        int loop = static_cast<int>(std::ceil((m_buffer.size() + sizeof(Header)) / static_cast<double>(m_maxPacketSize)));
        std::vector<std::string> buffers(loop);
        int realPackSize = m_maxPacketSize - sizeof(Header);
        for (int i = 0; i < loop; i++) //分包，每包最大m_maxPacketSize
        {
            bool lastPack    = i == loop - 1;
            auto currentSize = static_cast<int>(lastPack ? m_buffer.size() - i * realPackSize : realPackSize);
            buffers[i].append(Header(m_msgId, calcTag(), currentSize, lastPack));
            buffers[i].append(m_buffer.c_str() + i * realPackSize, currentSize);
            if (auto multiOf4 = buffers[i].size() % 4; multiOf4) //每包必须为4的倍数
            {
                std::string empty;
                empty.resize(4 - multiOf4, 0x00);
                buffers[i].append(std::move(empty));
            }
        }
        return buffers;
    }

private:
    unsigned char calcTag()
    {
        m_refTag++;
        if (m_refTag == 0)
            m_refTag++;
        return m_refTag;
    }
    std::string m_buffer;
    unsigned char& m_refTag;
    unsigned char m_msgId;
    unsigned short m_maxPacketSize;
};

class BulkRequest
{
#pragma pack(push)
#pragma pack(1)
    struct Header
    {
        unsigned char m_msgId { USBTMC_MSGID_REQUEST_DEV_DEP_MSG_IN };
        unsigned char m_tag;                          // 每次发送增加1, 1<=tag<=255
        unsigned char m_tagInverse;                   // m_tag的补码
        unsigned char m_reserved1 { 0x00 };           // 必须为0x00s
        unsigned int m_transferSize;                  // 要接收的数据大小，传输最大可传输字节时，必须为0
        unsigned char m_bmTransferAttributes { 0x0 }; // 0x02 表示USBTMC传输必须终止在termchar上，否则忽略
        unsigned char m_termChar { 0x0 };
        unsigned short m_reserved { 0x0 };
    };
#pragma pack(pop)
public:
    inline BulkRequest(unsigned char& tag, unsigned int transferSize) : m_tag(tag), m_transferSize(transferSize) {}
    inline ~BulkRequest() {}

    inline operator std::string() noexcept
    {
        Header header;
        header.m_tag          = ++m_tag;
        header.m_tagInverse   = ~header.m_tag;
        header.m_transferSize = m_transferSize;
        return std::string(reinterpret_cast<char*>(&header), sizeof(header));
    }

private:
    unsigned char& m_tag;
    unsigned int m_transferSize;
};

class BulkIn
{
public:
#pragma pack(push)
#pragma pack(1)
    struct Header
    {
        unsigned char m_msgId;
        unsigned char m_tag;                      // 每次发送增加1, 1<=tag<=255
        unsigned char m_tagInverse;               // m_tag的补码
        unsigned char m_reserved1 { 0x00 };       // 必须为0x00
        unsigned int m_transferSize;              // 传输数据大小
        bool m_bmTransfer;                        // EOM 是否最后一包
        unsigned char m_reserved2[3] { 0, 0, 0 }; // 填充字节
    };
#pragma pack(pop)

    inline BulkIn() noexcept {}
    inline ~BulkIn() noexcept {}
    // 字节数不足返回false，数据解析错误抛出异常
    bool parse(std::string& buffer, size_t requestSize)
    {
        if (buffer.size() < sizeof(Header))
            return false;
        auto header = reinterpret_cast<const Header*>(buffer.c_str());

        if (header->m_transferSize + sizeof(Header) > buffer.size())
            return false;
        m_buffer.append(buffer.c_str() + sizeof(Header), header->m_transferSize);
        auto ret = header->m_bmTransfer || requestSize == m_buffer.size();
        m_eom    = header->m_bmTransfer;
        buffer.erase(buffer.begin(), buffer.begin() + header->m_transferSize + sizeof(Header));
        return ret;
    }

    inline operator std::string() const&& { return std::move(m_buffer); }
    inline bool eom() const { return m_eom; }

private:
    std::string m_buffer;
    bool m_eom { false };
};
} // namespace OpenVisa
