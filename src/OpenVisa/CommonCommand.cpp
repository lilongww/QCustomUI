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
#include "CommonCommand.h"
#include "Utils.h"

namespace OpenVisa
{
/*!
    \class      OpenVisa::Object::CommonCommand
    \brief      IEEE488.2公共指令集API接口.
    \ingroup    openvisa
    \inmodule   OpenVisa
    \inheaderfile CommonCommand.h
*/

/*!
    \brief      清除事件寄存器位和错误队列.
*/
void Object::CommonCommand::cls() { send("*CLS"); }

/*!
    \brief      事件状态启用命令 \a en,
                为标准事件寄存器组启用使能寄存器中的位。
                随后将所选位报告给状态字节寄存器的 5 位。
                使能寄存器可定义事件寄存器中的哪些位将被报告给“状态字节”寄存器组。
                使能寄存器是可读写的。 (*ESE).
    \sa         ese
*/
void Object::CommonCommand::setEse(int en) { send("*ESE {}", en); }

/*!
    \brief      事件状态查询命令。(*ESE?).
    \sa         setEse
*/
int Object::CommonCommand::ese() { return OpenVisa::decode<int>(query("*ESE?")); }

/*!
    \brief      标准事件状态寄存器查询。
                查询标准事件寄存器组的事件寄存器。
                事件寄存器是只读寄存器，从条件寄存器锁存事件。
                设置事件位时，将忽略与该位相对应的随后发生的事件。(*ESR?).
*/
int Object::CommonCommand::esr() { return OpenVisa::decode<int>(query("*ESR?")); }

/*!
    \brief      标识查询。 返回仪器的标识字符串.
*/
std::string Object::CommonCommand::idn() { return query("*IDN?"); }

/*!
    \brief      在完成当前操作时，在标准事件寄存器中设置“操作完成”（0 位）。(*OPC).
    \sa         opc
*/
void Object::CommonCommand::setOpc() { send("*OPC"); }

/*!
    \brief      在所有的未决命令完成后，将 1 返回到输出缓冲器。 在该命令完成之前，无法执行其他命令。
    \sa         setOpc
*/
bool Object::CommonCommand::opc() { return OpenVisa::decode<bool>(query("*OPC?")); }

/*!
    \brief      返回标识任何已安装选件的字符串。
*/
std::vector<std::string> Object::CommonCommand::opt() { return OpenVisa::split(query("*OPT?"), ","); }

/*!
    \brief      服务请求启用 \a status,
                为状态字节寄存器组启用使能寄存器中的位。
                使能寄存器可定义事件寄存器中的哪些位将被报告给“状态字节”寄存器组。
                使能寄存器是可读写的。(*SRE).
    \sa         sre
*/
void Object::CommonCommand::setSre(int status) { send("*SRE {}", status); }

/*!
    \brief      查询服务请求是否启用 (*SRE?).
    \sa         setSre
*/
int Object::CommonCommand::sre() { return OpenVisa::decode<int>(query("*SRE?")); }

/*!
    \brief      读取状态字节查询。
                查询状态字节寄存器组的条件寄存器并返回一个十进制值，该值等于寄存器中设置的所有位的二进制加权值总和。
                条件寄存器可不间断地监控仪器的状态。 条件寄存器位实时更新；它们既不锁定亦无缓冲。
*/
StatusByteRegisters Object::CommonCommand::stb() { return OpenVisa::decode<StatusByteRegisters>(query("*STB?")); }

/*!
    \brief      执行仪器的基本自检并返回通过/失败指示.
*/
bool Object::CommonCommand::tst() { return !OpenVisa::decode<int>(query("*TST?")); }

/*!
    \brief      配置仪器的输出缓冲区，等待所有未决操作完成之后，再通过接口执行任何其他命令。
*/
void Object::CommonCommand::wai() { send("*WAI"); }

/*!
    \brief      调用寄存器保存文件 \a i.
*/
void Object::CommonCommand::rcl(int i) { send("*RCL ", i); }

/*!
    \brief      重置仪表状态.
*/
void Object::CommonCommand::rst() { send("*RST"); }
} // namespace OpenVisa
