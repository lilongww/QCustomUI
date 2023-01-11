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

#include <boost/endian.hpp>

#include <bit>

namespace OpenVisa
{

template<typename T>
constexpr T toBigEndian(const T src)
{
    if constexpr (std::endian::native == std::endian::big || sizeof(T) == 1)
        return src;
    else
    {
        return boost::endian::native_to_big(src);
    }
}

template<typename T>
constexpr T fromBigEndian(const T src)
{
    if constexpr (std::endian::native == std::endian::big || sizeof(T) == 1)
        return src;
    else
        return boost::endian::big_to_native(src);
}
} // namespace OpenVisa