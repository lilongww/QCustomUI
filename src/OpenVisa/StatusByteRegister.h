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

#include "Flags.h"

namespace OpenVisa
{
enum class StatusByteRegister : unsigned short
{
    Unused1                  = 0x0001,
    Unused2                  = 0x0002,
    ErrorQueue               = 0x0004,
    QuestionableDataSummary  = 0x0008,
    MessageAvailable         = 0x0010,
    StandardEventSummary     = 0x0020,
    MasterSummary            = 0x0040,
    StandardOperationSummary = 0x0080
};

using StatusByteRegisters = Flags<StatusByteRegister>;
} // namespace OpenVisa