#pragma once

#include <string>
#include <vector>

namespace OpenVisa
{
class SerialPortInfo
{
public:
    inline SerialPortInfo() noexcept {}
    inline ~SerialPortInfo() noexcept {}

    [[nodiscard]] inline const std::string& portName() const noexcept { return m_portName; }
    [[nodiscard]] inline const std::string& description() const noexcept { return m_description; }
    [[nodiscard]] inline const std::string& hardwareID() const noexcept { return m_hardwareID; }
    [[nodiscard]] inline const std::string& manufacturer() const noexcept { return m_manufacturer; }
    [[nodiscard]] inline bool hasVendorId() const noexcept { return m_hasVid; }
    [[nodiscard]] inline unsigned short vendorId() const noexcept { return m_vid; }
    [[nodiscard]] inline bool hasProductId() const noexcept { return m_hasPid; }
    [[nodiscard]] inline unsigned short productId() const noexcept { return m_pid; }
    [[nodiscard]] inline const std::string& serialNumber() const noexcept { return m_serialNumber; }
    static std::vector<SerialPortInfo> listPorts();

private:
    inline void setPortName(std::string_view name) noexcept { m_portName = name; }
    inline void setDescrption(std::string_view description) noexcept { m_description = description; }
    inline void setHardwareID(std::string_view id) noexcept { m_hardwareID = id; }
    inline void setManufacturer(std::string_view manufacturer) noexcept { m_manufacturer = manufacturer; }
    inline void setVendorId(unsigned short vid) noexcept;
    inline void setProductId(unsigned short pid) noexcept;
    inline void setSerialNumber(std::string_view sn) noexcept { m_serialNumber = sn; }

protected:
    std::string m_portName;
    std::string m_description;
    std::string m_hardwareID;
    std::string m_manufacturer;
    bool m_hasVid { false };
    bool m_hasPid { false };
    unsigned short m_vid { 0 };
    unsigned short m_pid { 0 };
    std::string m_serialNumber;
};

inline void SerialPortInfo::setVendorId(unsigned short vid) noexcept
{
    m_hasVid = true;
    m_vid    = vid;
}

inline void SerialPortInfo::setProductId(unsigned short pid) noexcept
{
    m_hasPid = true;
    m_pid    = pid;
}

} // namespace OpenVisa
