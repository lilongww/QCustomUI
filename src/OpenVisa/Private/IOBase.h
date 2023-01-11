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

#include <string>

namespace OpenVisa
{
class IOBase
{
public:
    inline IOBase(Object::Attribute const& attr) : m_attr(attr) {}
    virtual ~IOBase() noexcept                         = default;
    virtual void send(const std::string& buffer) const = 0;
    virtual std::string readAll() const                = 0;
    virtual std::string read(size_t size) const        = 0;
    virtual void close() noexcept                      = 0;
    virtual bool connected() const noexcept            = 0;
    virtual size_t avalible() const noexcept           = 0;

protected:
    Object::Attribute const& m_attr;
};
} // namespace OpenVisa
