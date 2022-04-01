#include "CommonCommand.h"
#include "Utils.h"

namespace OpenVisa
{
/*!
    \class      OpenVisa::Object::CommonCommand
    \brief      IEEE488.2公共指令集API接口.
    \ingroup    openvisa
    \inmodule   InstrumentDrivers
    \inheaderfile CommonCommand.h
*/

/*!
    \brief      清除事件寄存器位和错误队列.
*/
void Object::CommonCommand::cls() { m_parent->send("*CLS"); }

/*!
    \brief      事件状态启用命令 \a en,
                为标准事件寄存器组启用使能寄存器中的位。
                随后将所选位报告给状态字节寄存器的 5 位。
                使能寄存器可定义事件寄存器中的哪些位将被报告给“状态字节”寄存器组。
                使能寄存器是可读写的。 (*ESE).
    \sa         ese
*/
void Object::CommonCommand::setEse(int en) { m_parent->send("*ESE {}", en); }

/*!
    \brief      事件状态查询命令。(*ESE?).
    \sa         setEse
*/
int Object::CommonCommand::ese()
{
    m_parent->send("*ESE?");
    return OpenVisa::decode<int>(m_parent->readAll());
}

/*!
    \brief      标准事件状态寄存器查询。
                查询标准事件寄存器组的事件寄存器。
                事件寄存器是只读寄存器，从条件寄存器锁存事件。
                设置事件位时，将忽略与该位相对应的随后发生的事件。(*ESR?).
*/
int Object::CommonCommand::esr()
{
    m_parent->send("*ESR?");
    return OpenVisa::decode<int>(m_parent->readAll());
}

/*!
    \brief      标识查询。 返回仪器的标识字符串.
*/
std::string Object::CommonCommand::idn()
{
    m_parent->send("*IDN?");
    return m_parent->readAll();
}

/*!
    \brief      在完成当前操作时，在标准事件寄存器中设置“操作完成”（0 位）。(*OPC).
    \sa         opc
*/
void Object::CommonCommand::setOpc() { m_parent->send("*OPC"); }

/*!
    \brief      在所有的未决命令完成后，将 1 返回到输出缓冲器。 在该命令完成之前，无法执行其他命令。
    \sa         setOpc
*/
bool Object::CommonCommand::opc()
{
    m_parent->send("*OPC?");
    return OpenVisa::decode<bool>(m_parent->readAll());
}

/*!
    \brief      返回标识任何已安装选件的字符串。
*/
std::vector<std::string> Object::CommonCommand::opt()
{
    m_parent->send("*OPT?");
    return OpenVisa::split(m_parent->readAll(), ",");
}

/*!
    \brief      服务请求启用 \a status,
                为状态字节寄存器组启用使能寄存器中的位。
                使能寄存器可定义事件寄存器中的哪些位将被报告给“状态字节”寄存器组。
                使能寄存器是可读写的。(*SRE).
    \sa         sre
*/
void Object::CommonCommand::setSre(int status) { m_parent->send("*SRE {}", status); }

/*!
    \brief      查询服务请求是否启用 (*SRE?).
    \sa         setSre
*/
int Object::CommonCommand::sre()
{
    m_parent->send("*SRE?");
    return OpenVisa::decode<int>(m_parent->readAll());
}

/*!
    \brief      读取状态字节查询。
                查询状态字节寄存器组的条件寄存器并返回一个十进制值，该值等于寄存器中设置的所有位的二进制加权值总和。
                条件寄存器可不间断地监控仪器的状态。 条件寄存器位实时更新；它们既不锁定亦无缓冲。
*/
int Object::CommonCommand::stb()
{
    m_parent->send("*STB?");
    return OpenVisa::decode<int>(m_parent->readAll());
}

/*!
    \brief      执行仪器的基本自检并返回通过/失败指示.
*/
bool Object::CommonCommand::tst()
{
    m_parent->send("*TST?");
    return !OpenVisa::decode<int>(m_parent->readAll());
}

/*!
    \brief      配置仪器的输出缓冲区，等待所有未决操作完成之后，再通过接口执行任何其他命令。
*/
void Object::CommonCommand::wai() { m_parent->send("*WAI"); }

/*!
    \brief      调用寄存器保存文件 \a i.
*/
void Object::CommonCommand::rcl(int i) { m_parent->send("*RCL ", i); }

/*!
    \brief      重置仪表状态.
*/
void Object::CommonCommand::rst() { m_parent->send("*RST"); }
} // namespace OpenVisa
