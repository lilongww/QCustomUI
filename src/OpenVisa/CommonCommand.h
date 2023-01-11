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
#include "StatusByteRegister.h"

#include <vector>

namespace OpenVisa
{
class OPENVISA_EXPORT Object::CommonCommand
{
public:
    void cls();
    void setEse(int status);
    int ese();
    int esr();
    std::string idn();
    void setOpc();
    bool opc();
    std::vector<std::string> opt();
    void rst();
    void setSre(int status);
    int sre();
    StatusByteRegisters stb();
    bool tst();
    void wai();
    void rcl(int i);

private:
    inline CommonCommand(Object* parent) : m_parent(parent) {};
    friend Object;
    Object* m_parent;
};
} // namespace OpenVisa
