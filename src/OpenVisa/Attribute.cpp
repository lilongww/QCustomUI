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
#include "Attribute.h"

namespace OpenVisa
{
struct Object::Attribute::Impl
{
    std::string termChars { "\n" };
    std::chrono::milliseconds timeout { 2000 };
    bool autoAppendTerminalChars { true };
};

/*!
    \class      OpenVisa::Object::Attribute
    \brief      驱动属性集.
    \ingroup    openvisa
    \inmodule   OpenVisa
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

/*!
    \brief      设置通信超时时间 \a timeout.
    \sa         timeout
*/
void Object::Attribute::setTimeout(const std::chrono::milliseconds& timeout) { m_impl->timeout = timeout; }

/*!
    \brief      返回通信超时时间.
    \sa         setTimeout
*/
const std::chrono::milliseconds& Object::Attribute::timeout() const { return m_impl->timeout; }

/*!
    \brief      设置是否启用发送数据时是否自动添加终结符 \a en, 默认为true.
    \sa         autoAppendTerminalChars
*/
void Object::Attribute::setAutoAppendTerminalChars(bool en) { m_impl->autoAppendTerminalChars = en; }

/*!
    \brief      返回是否启用发送数据时是否自动添加终结符.
    \sa         setAutoAppendTerminalChars
*/
bool Object::Attribute::autoAppendTerminalChars() const { return m_impl->autoAppendTerminalChars; }

} // namespace OpenVisa
