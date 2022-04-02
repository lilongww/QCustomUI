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
#include "Attribute.h"

namespace OpenVisa
{
struct Object::Attribute::Impl
{
    std::string termChars { "\n" };
};

/*!
    \class      OpenVisa::Object::Attribute
    \brief      驱动属性集.
    \ingroup    openvisa
    \inmodule   InstrumentDrivers
    \inheaderfile Attribute.h
*/

/*!
    \brief      构造函数.
*/
Object::Attribute::Attribute() : m_impl(std::make_unique<Impl>()) {}

/*!
    \brief      析构函数.
*/
Object::Attribute::~Attribute() {}

/*!
    \brief      设置终结符 \a chars, 默认为 '\n'(0x0A).
    \sa         terminalChars
*/
void Object::Attribute::setTerminalChars(std::string_view chars) { m_impl->termChars = chars; }

/*!
    \brief      返回终结符.
    \sa         setTerminalChars
*/
const std::string& Object::Attribute::terminalChars() const { return m_impl->termChars; }

} // namespace OpenVisa
