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
#pragma once

#include "openvisa_global.h"

#include <string>
#include <vector>

namespace OpenVisa
{
class OPENVISA_EXPORT SerialPortInfo
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
