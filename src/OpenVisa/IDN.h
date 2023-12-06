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

#include <ranges>
#include <string>
#include <vector>

namespace OpenVisa
{
class IDN
{
public:
    inline IDN(const std::string& source)
    {
        auto view =
            std::views::split(source, ',') | std::views::transform([](const auto& view) { return std::string(view.begin(), view.end()); });
        m_datas = std::vector<std::string>(view.begin(), view.end());
    }
    inline ~IDN() {}
    inline const std::string& manufacturer() const { return m_datas[0]; }
    inline const std::string& model() const { return m_datas[1]; }
    inline const std::string& serialNumber() const { return m_datas[2]; }
    inline const std::string& firmwareRevision() const { return m_datas[3]; }

private:
    std::vector<std::string> m_datas;
};
} // namespace OpenVisa