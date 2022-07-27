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
#include "Utils.h"

#include <ranges>
#include <regex>

using namespace std::literals::string_view_literals;

namespace OpenVisa
{
constexpr auto BoolalphaChars = "ONOFF10+"sv;
constexpr auto DoubleView = std::views::take_while([](char ch) { return ch <= '9' && ch >= '0' || ch == '.' || ch == '+' || ch == '-'; });
constexpr auto IntView    = std::views::take_while([](char ch) { return ch <= '9' && ch >= '0' || ch == '+' || ch == '-'; });
void VisaAdl<bool>::fromScpi(const std::string& ret, bool& val)
{
    std::string temp;
    std::ranges::copy(ret | std::views::take_while([](char ch) { return BoolalphaChars.find_first_of(ch) != std::string_view::npos; }),
                      std::back_inserter(temp));
    if (temp == "ON")
        val = true;
    else if (temp == "OFF")
        val = false;
    else if (temp == "1" || temp == "+1")
        val = true;
    else if (temp == "0")
        val = false;
    else
        throw std::exception((std::string("Visa::parseBool Unknown ret:") + temp).c_str());
}

void VisaAdl<double>::fromScpi(const std::string& ret, double& val)
{
    std::string temp;
    std::ranges::copy(ret | DoubleView, std::back_inserter(temp));
    val = std::stod(temp);
}

std::string VisaAdl<double>::toScpi(double val) { return std::to_string(val); }

void VisaAdl<int>::fromScpi(const std::string& ret, int& val)
{
    std::string temp;
    std::ranges::copy(ret | IntView, std::back_inserter(temp));
    val = std::stoi(ret);
}

std::string VisaAdl<int>::toScpi(int val) { return std::to_string(val); }

std::string VisaAdl<bool>::toScpi(bool source) { return source ? "ON" : "OFF"; }

OPENVISA_EXPORT std::vector<std::string> split(const std::string& source, const std::string& s)
{
    std::regex re { s };
    return std::vector<std::string> { std::sregex_token_iterator(source.begin(), source.end(), re, -1), std::sregex_token_iterator() };
}

void VisaAdl<StatusByteRegisters>::fromScpi(const std::string& ret, StatusByteRegisters& val)
{
    std::string temp;
    std::ranges::copy(ret | IntView, std::back_inserter(temp));
    val = StatusByteRegisters(std::stoi(temp));
}

std::string VisaAdl<StatusByteRegisters>::toScpi(StatusByteRegisters source)
{
    return std::to_string(static_cast<unsigned short>(source.flags()));
}
} // namespace OpenVisa
