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

#define OPENVISA_VERSION_MAJOR 0
#define OPENVISA_VERSION_MINOR 5
#define OPENVISA_VERSION_MICRO 0

#define _OPENVISA_TOSTR(x) #x
#define _OPENVISA_TO_STR(x) _OPENVISA_TOSTR(x)

#define OPENVISA_VERSION                                                                                                                   \
    _OPENVISA_TO_STR(OPENVISA_VERSION_MAJOR) "." _OPENVISA_TO_STR(OPENVISA_VERSION_MINOR) "." _OPENVISA_TO_STR(OPENVISA_VERSION_MICRO)
