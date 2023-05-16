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

#include "Object.h"

#include <chrono>
#include <optional>

namespace OpenVisa
{
enum BaudRate
{
    _1200   = 1200,
    _2400   = 2400,
    _4800   = 4800,
    _9600   = 9600,
    _19200  = 19200,
    _38400  = 38400,
    _57600  = 57600,
    _115200 = 115200
};

enum class DataBits
{
    Data5 = 5,
    Data6,
    Data7,
    Data8
};

enum class FlowControl
{
    None,
    Software,
    Hardware
};

enum class Parity
{
    None,
    Odd,
    Even
};

enum class StopBits
{
    One,
    OnePointFive,
    Two
};

class IOBase;
class OPENVISA_EXPORT Object::Attribute
{
public:
    void setTerminalChars(std::string_view chars);
    [[nodiscard]] const std::string& terminalChars() const;
    void setTerminalCharsEnable(bool enable);
    [[nodiscard]] bool terminalCharsEnable() const;
    void setTimeout(const std::chrono::milliseconds& timeout);
    [[nodiscard]] const std::chrono::milliseconds& timeout() const;
    void setAutoAppendTerminalChars(bool en);
    [[nodiscard]] bool autoAppendTerminalChars() const;
    void setBaudRate(unsigned int baud);
    [[nodiscard]] std::optional<unsigned int> baudRate() const;
    void setDataBits(DataBits bits);
    [[nodiscard]] std::optional<DataBits> dataBits() const;
    void setFlowControl(FlowControl fc);
    [[nodiscard]] std::optional<FlowControl> flowControl() const;
    void setParity(Parity p);
    [[nodiscard]] std::optional<Parity> parity() const;
    void setStopBits(StopBits bits);
    [[nodiscard]] std::optional<StopBits> stopBits() const;
    void setDeviceName(std::string_view name);
    [[nodiscard]] const std::string& deviceName() const;

protected:
    Attribute(std::shared_ptr<IOBase>* io);
    ~Attribute();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend Object;
    friend class UsbTmc;
};
} // namespace OpenVisa
