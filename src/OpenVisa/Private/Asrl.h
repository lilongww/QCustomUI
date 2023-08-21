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
#include "../Attribute.h"

namespace OpenVisa
{
struct Asrl
{
    std::string portName;
    unsigned int baud { 9600 };
    DataBits dataBits { DataBits::Data8 };
    FlowControl flowControl { FlowControl::None };
    Parity parity { Parity::None };
    StopBits stopBits { StopBits::One };
    inline bool isValid() const { return baud != 0; }
};
} // namespace OpenVisa